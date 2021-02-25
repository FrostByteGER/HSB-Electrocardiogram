#pragma once

struct Heartbeat
{
    Heartbeat(const size_t frameStartMs, const size_t frameEndMs) : frameStartMs(frameStartMs), frameEndMs(frameEndMs)
    {
    }

    size_t frameStartMs;
    size_t frameEndMs;
};