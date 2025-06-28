/*********************************************************************
 *   File: FrequencyThread.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_FREQUENCY_THREAD_HPP
#define K_FREQUENCY_THREAD_HPP

#include <chrono>
#include <iostream>
#include "Core/TypesDefinition.hpp"

class FFrequencyThread
{
public:
    FFrequencyThread(double hz, TFunction<void()> callback)
        : m_Hz(hz), m_Callback(callback), m_Running(false)
    {
    }

    ~FFrequencyThread()
    {
        Stop();
    }

    void Start()
    {
        if (m_Running)
            return;

        m_Running = true;
        m_Thread = FThread([this]()
            {
                using namespace std::chrono;

                const double intervalMs = 1000.0 / m_Hz;
                auto nextTick = high_resolution_clock::now();

                while (m_Running)
                {
                    auto startTime = high_resolution_clock::now();

                    // Executa o callback
                    m_Callback();

                    nextTick += milliseconds((int64_t)intervalMs);

                    auto now = high_resolution_clock::now();
                    auto sleepDuration = duration_cast<milliseconds>(nextTick - now);

                    if (sleepDuration.count() > 0)
                    {
                        std::this_thread::sleep_for(sleepDuration);
                    }
                    else
                    {
                        // Est� atrasado, ajusta o pr�ximo tick
                        nextTick = high_resolution_clock::now();
                    }
                }
            });
    }

    void Stop()
    {
        if (!m_Running)
            return;

        m_Running = false;
        if (m_Thread.joinable())
        {
            m_Thread.join();
        }
    }

private:
    double m_Hz;
    TFunction<void()> m_Callback;
    std::atomic<bool> m_Running;
    FThread m_Thread;
};

#endif // K_FREQUENCY_THREAD_HPP

