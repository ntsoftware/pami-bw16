#include <Arduino.h>
#include <cstdint>
#include <stdio.h>
#include <string.h>
#include "capture.h"
#include "data_types.h"
#include "utils/debug.h"

#define MAX_CAPTURE_FILES 100

Capture::Capture()
{
}

Capture::~Capture()
{
}

void Capture::begin()
{
    for (int i = 0; i < MAX_CAPTURE_FILES; ++i) {
        char path[16];
        snprintf(path, sizeof(path), "cap%04d.bin", i);

        if (hal::sd.open(path, O_CREAT | O_EXCL | O_WRONLY, file)) {
            dbg.printf("created capture file %s\n", path);
            return;
        }
    }

    dbg.printf("failed to create capture file\n");
}

void Capture::end()
{
    file.close();
}

void Capture::write_header()
{
    static const uint8_t buf[4] = {0x01, 0x00, 0x00, 0x00};
    file.write(buf, sizeof(buf));
    file.sync();
}

void Capture::write_frame(DataFrame &frame)
{
    switch (frame.type) {
        case FRAME_TYPE_MOVE:
            write_move_frame(frame.move);
            break;
        case FRAME_TYPE_SCAN:
            write_scan_frame(frame.scan);
            break;
        case FRAME_TYPE_ESTIMATED_POSE:
            write_estimated_pose_frame(frame.estimated_pose);
            break;
        case FRAME_TYPE_CURRENT_POSE:
            write_current_pose_frame(frame.current_pose);
            break;
        case FRAME_TYPE_PATH:
            write_path_frame(frame.path);
            break;
        case FRAME_TYPE_MOTOR:
            write_motor_frame(frame.motor);
            break;
        default:
            break;
    }
}

void Capture::write_move_frame(DataFrame::Move &frame)
{
    const uint8_t type = FRAME_TYPE_MOVE;
    file.write(&type, sizeof(type));
    file.write(&frame.t, sizeof(frame.t));
    file.write(&frame.delta_x, sizeof(frame.delta_x));
    file.write(&frame.delta_y, sizeof(frame.delta_y));
    file.write(&frame.delta_theta, sizeof(frame.delta_theta));
    file.sync();
}

void Capture::write_scan_frame(DataFrame::Scan &frame)
{
    const uint8_t type = FRAME_TYPE_SCAN;
    file.write(&type, sizeof(type));
    file.write(&frame.t, sizeof(frame.t));
    file.write(&frame.border_point_count, sizeof(frame.border_point_count));
    file.write(&frame.obstacle_point_count, sizeof(frame.obstacle_point_count));
    file.write(frame.border_points, frame.border_point_count * sizeof(Point2d));
    file.write(frame.obstacle_points, frame.obstacle_point_count * sizeof(Point2d));
    file.sync();
}

void Capture::write_estimated_pose_frame(DataFrame::EstimatedPose &frame)
{
    const uint8_t type = FRAME_TYPE_ESTIMATED_POSE;
    file.write(&type, sizeof(type));
    file.write(&frame.t, sizeof(frame.t));
    file.write(&frame.x, sizeof(frame.x));
    file.write(&frame.y, sizeof(frame.y));
    file.write(&frame.theta, sizeof(frame.theta));
    file.sync();
}

void Capture::write_current_pose_frame(DataFrame::CurrentPose &frame)
{
    const uint8_t type = FRAME_TYPE_CURRENT_POSE;
    file.write(&type, sizeof(type));
    file.write(&frame.t, sizeof(frame.t));
    file.write(&frame.x, sizeof(frame.x));
    file.write(&frame.y, sizeof(frame.y));
    file.write(&frame.theta, sizeof(frame.theta));
    file.sync();
}

void Capture::write_path_frame(DataFrame::Path &frame)
{
    const uint8_t type = FRAME_TYPE_PATH;
    file.write(&type, sizeof(type));
    file.write(&frame.t, sizeof(frame.t));
    file.write(&frame.point_count, sizeof(frame.point_count));
    file.write((const uint8_t *) frame.points, frame.point_count * sizeof(PathPoint));
    file.sync();
}

void Capture::write_motor_frame(DataFrame::Motor &frame)
{
    const uint8_t type = FRAME_TYPE_MOTOR;
    file.write(&type, sizeof(type));
    file.write(&frame.t, sizeof(frame.t));
    file.write(&frame.speed_a, sizeof(frame.speed_a));
    file.write(&frame.speed_b, sizeof(frame.speed_b));
    file.write(&frame.speed_c, sizeof(frame.speed_c));
    file.sync();
}
