#include "data.h"
#include "data_types.h"
#include "hal/teensy.h"
#include "utils/debug.h"

#define MAX_BORDER_POINTS 360
#define MAX_OBSTACLE_POINTS 360
#define MAX_PATH_POINTS 500

static Point2d border_points[MAX_BORDER_POINTS];
static Point2d obstacle_points[MAX_OBSTACLE_POINTS];
static PathPoint path_points[MAX_PATH_POINTS];

static void dump_points(const Point2d *points, size_t n);
static void dump_path(const PathPoint *points, size_t n);

static uint16_t min(uint16_t a, uint16_t b)
{
    return a < b ? a : b;
}

Data::Data() : rx_checksum(0), tx_checksum(0)
{
}

void Data::send_heartbeat(const Heartbeat &heartbeat)
{
    send_frame_header(FRAME_TYPE_HEARTBEAT);
    send_u8(heartbeat.robot_mode);
    send_u8(heartbeat.team_color);
    send_u8(heartbeat.goal_zone);
    send_u32(heartbeat.game_time);
    send_frame_checksum();
}

void Data::recv_frame(DataFrame &out)
{
    while (1) {
        switch (recv_frame_header()) {
            case FRAME_TYPE_MOVE:
                if (recv_move_frame(out)) return;
                break;
            case FRAME_TYPE_SCAN:
                if (recv_scan_frame(out)) return;
                break;
            case FRAME_TYPE_ESTIMATED_POSE:
                if (recv_estimated_pose_frame(out)) return;
                break;
            case FRAME_TYPE_CURRENT_POSE:
                if (recv_current_pose_frame(out)) return;
                break;
            case FRAME_TYPE_PATH:
                if (recv_path_frame(out)) return;
                break;
            case FRAME_TYPE_MOTOR:
                if (recv_motor_frame(out)) return;
                break;
            default:
                continue;
        }
    }
}

bool Data::recv_move_frame(DataFrame &out)
{
    uint32_t t;
    int16_t delta_x;
    int16_t delta_y;
    int16_t delta_theta;

    if (!recv_u32(t)) return false;
    if (!recv_i16(delta_x)) return false;
    if (!recv_i16(delta_y)) return false;
    if (!recv_i16(delta_theta)) return false;
    if (!recv_frame_checksum()) return false;

    out.type = FRAME_TYPE_MOVE;
    out.move.t = t;
    out.move.delta_x = delta_x;
    out.move.delta_y = delta_y;
    out.move.delta_theta = delta_theta;
    return true;
}

bool Data::recv_scan_frame(DataFrame &out)
{
    uint32_t t;
    uint16_t border_point_count;
    uint16_t obstacle_point_count;

    if (!recv_u32(t)) return false;
    if (!recv_u16(border_point_count)) return false;
    if (!recv_u16(obstacle_point_count)) return false;
    if (!recv_buf_with_size((uint8_t *) border_points, 4 * border_point_count, 4 * MAX_BORDER_POINTS)) return false;
    if (!recv_buf_with_size((uint8_t *) obstacle_points, 4 * obstacle_point_count, 4 * MAX_OBSTACLE_POINTS)) return false;
    if (!recv_frame_checksum()) return false;

    out.type = FRAME_TYPE_SCAN;
    out.scan.t = t;
    out.scan.border_point_count = min(border_point_count, MAX_BORDER_POINTS);
    out.scan.obstacle_point_count = min(obstacle_point_count, MAX_OBSTACLE_POINTS);
    out.scan.border_points = border_points;
    out.scan.obstacle_points = obstacle_points;
    return true;
}

bool Data::recv_estimated_pose_frame(DataFrame &out)
{
    uint32_t t;
    uint16_t x;
    uint16_t y;
    int16_t theta;

    if (!recv_u32(t)) return false;
    if (!recv_u16(x)) return false;
    if (!recv_u16(y)) return false;
    if (!recv_i16(theta)) return false;
    if (!recv_frame_checksum()) return false;

    out.type = FRAME_TYPE_ESTIMATED_POSE;
    out.estimated_pose.t = t;
    out.estimated_pose.x = x;
    out.estimated_pose.y = y;
    out.estimated_pose.theta = theta;
    return true;
}

bool Data::recv_current_pose_frame(DataFrame &out)
{
    uint32_t t;
    uint16_t x;
    uint16_t y;
    int16_t theta;

    if (!recv_u32(t)) return false;
    if (!recv_u16(x)) return false;
    if (!recv_u16(y)) return false;
    if (!recv_i16(theta)) return false;
    if (!recv_frame_checksum()) return false;

    out.type = FRAME_TYPE_CURRENT_POSE;
    out.current_pose.t = t;
    out.current_pose.x = x;
    out.current_pose.y = y;
    out.current_pose.theta = theta;
    return true;
}

bool Data::recv_path_frame(DataFrame &out)
{
    uint32_t t;
    uint16_t point_count;

    if (!recv_u32(t)) return false;
    if (!recv_u16(point_count)) return false;
    if (!recv_buf_with_size((uint8_t *) path_points, 2 * point_count, 2 * MAX_PATH_POINTS)) return false;
    if (!recv_frame_checksum()) return false;

    out.type = FRAME_TYPE_PATH;
    out.path.t = t;
    out.path.point_count = min(point_count, MAX_PATH_POINTS);
    out.path.points = path_points;
    return true;
}

bool Data::recv_motor_frame(DataFrame &out)
{
    uint32_t t;
    int16_t speed_a;
    int16_t speed_b;
    int16_t speed_c;

    if (!recv_u32(t)) return false;
    if (!recv_i16(speed_a)) return false;
    if (!recv_i16(speed_b)) return false;
    if (!recv_i16(speed_c)) return false;
    if (!recv_frame_checksum()) return false;

    out.type = FRAME_TYPE_MOTOR;
    out.motor.t = t;
    out.motor.speed_a = speed_a;
    out.motor.speed_b = speed_b;
    out.motor.speed_c = speed_c;
    return true;
}

int Data::recv_frame_header()
{
    rx_checksum = 0;

    uint8_t x;
    if (!recv_u8(x) || x != 0x55) return -1;
    if (!recv_u8(x) || x != 0xAA) return -1;

    uint8_t y;
    if (recv_u8(y)) {
        return y;
    } else {
        return -1;
    }
}

bool Data::recv_frame_checksum()
{
    uint8_t x;
    return recv_u8(x) && rx_checksum == 0;
}

bool Data::recv_buf(uint8_t *buf, size_t n)
{
    if (hal::teensy.recv(buf, n) == (int) n) {
        for (size_t i = 0; i < n; ++i) {
            rx_checksum += buf[i];
        }
        return true;
    } else {
        return false;
    }
}

// receive n bytes total, store maximum buf_size bytes into buf
bool Data::recv_buf_with_size(uint8_t *buf, size_t n, size_t buf_size)
{
    if (!recv_buf(buf, min(n, buf_size))) {
        return false;
    }
    for (size_t i = buf_size; i < n; ++i) {
        uint8_t x;
        if (!recv_u8(x)) {
            return false;
        }
    }
    return true;
}

bool Data::recv_u8(uint8_t &out)
{
    return recv_buf(&out, 1);
}

bool Data::recv_i16(int16_t &out)
{
    return recv_buf((uint8_t *) &out, 2);
}

bool Data::recv_u16(uint16_t &out)
{
    return recv_buf((uint8_t *) &out, 2);
}

bool Data::recv_u32(uint32_t &out)
{
    return recv_buf((uint8_t *) &out, 4);
}

void Data::send_frame_header(FrameType frame_type)
{
    const uint8_t buf[3] = {
        0x55,
        0xAA,
        (uint8_t) (frame_type),
    };
    tx_checksum = 0;
    send_buf(buf, sizeof(buf));
}

void Data::send_frame_checksum()
{
    uint8_t x = -tx_checksum;
    hal::teensy.send(&x, 1, true);
}

void Data::send_buf(const uint8_t *buf, size_t n)
{
    hal::teensy.send(buf, n, false);
    for (size_t i = 0; i < n; ++i) {
        tx_checksum += buf[i];
    }
}

void Data::send_u8(uint8_t x)
{
    send_buf(&x, 1);
}

void Data::send_u32(uint32_t x)
{
    send_buf((const uint8_t *) &x, 4);
}

DataFrame::DataFrame() : type(FRAME_TYPE_INVALID)
{
}

DataFrame::~DataFrame()
{
}

void DataFrame::dump()
{
    switch (type) {
        case FRAME_TYPE_MOVE:
            dbg.printf("[move frame]\n");
            dbg.printf("t=%d\n", move.t);
            dbg.printf("delta_x=%d\n", move.delta_x);
            dbg.printf("delta_y=%d\n", move.delta_y);
            dbg.printf("delta_theta=%d\n", move.delta_theta);
            break;
        case FRAME_TYPE_SCAN:
            dbg.printf("[scan frame]\n");
            dbg.printf("t=%d\n", scan.t);
            dbg.printf("border_points=");
            dump_points(scan.border_points, scan.border_point_count);
            dbg.printf("\n");
            dbg.printf("obstacle_points=");
            dump_points(scan.obstacle_points, scan.obstacle_point_count);
            dbg.printf("\n");
            break;
        case FRAME_TYPE_ESTIMATED_POSE:
            dbg.printf("[estimated pose frame]\n");
            dbg.printf("t=%d\n", estimated_pose.t);
            dbg.printf("x=%d\n", estimated_pose.x);
            dbg.printf("y=%d\n", estimated_pose.y);
            dbg.printf("theta=%d\n", estimated_pose.theta);
            break;
        case FRAME_TYPE_CURRENT_POSE:
            dbg.printf("[current pose frame]\n");
            dbg.printf("t=%d\n", current_pose.t);
            dbg.printf("x=%d\n", current_pose.x);
            dbg.printf("y=%d\n", current_pose.y);
            dbg.printf("theta=%d\n", current_pose.theta);
            break;
        case FRAME_TYPE_PATH:
            dbg.printf("[path frame]\n");
            dbg.printf("t=%d\n", estimated_pose.t);
            dbg.printf("path_points=");
            dump_path(path.points, path.point_count);
            dbg.printf("\n");
            break;
        case FRAME_TYPE_MOTOR:
            dbg.printf("[motor frame]\n");
            dbg.printf("t=%d\n", motor.t);
            dbg.printf("speed_a=%d\n", motor.speed_a);
            dbg.printf("speed_b=%d\n", motor.speed_b);
            dbg.printf("speed_c=%d\n", motor.speed_c);
            break;
        default:
            dbg.printf("[unknown frame (%d)]\n", type);
            break;
    }
}

static void dump_points(const Point2d *points, size_t n)
{
    dbg.printf("[");
    for (size_t i = 0; i < n; ++i) {
        dbg.printf("(x=%d,y=%d)", points[i].x, points[i].y);
        if (i != n - 1)
            dbg.printf(",");
    }
    dbg.printf("]");
}

static void dump_path(const PathPoint *points, size_t n)
{
    dbg.printf("[");
    for (size_t i = 0; i < n; ++i) {
        dbg.printf("(ix=%d,iy=%d)", points[i].ix, points[i].iy);
        if (i != n - 1)
            dbg.printf(",");
    }
    dbg.printf("]");

}
