#pragma once

#include <cinttypes>

// Type of targets
enum struct TargetType
{
    Satellite,
    Interference,
    Undefined
};

enum struct PolarizationType
{
    Vertical,
    Horizontal,
    Elliptic,
    Undefined
};

// Single target description
struct Target
{
    int id;

    // Coordinate in UV space
    double u;
    double v;
    double r;

    PolarizationType polarization;
    int search_area_id;
    double sigma;
    int channel_id;
    std::uint16_t power;
    double freq_start;
    double freq_width;

    TargetType type;
};
