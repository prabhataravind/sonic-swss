#pragma once

#include "orch.h"

struct DashTaskResult
{
    bool success;  // Whether the operation was successful
    bool retry;    // Whether the item should be kept in consumer for retry

    // Operation succeeded, remove from consumer
    static DashTaskResult ok() { return {true, false}; }

    // Operation failed but can be retried later, keep in consumer
    static DashTaskResult retryLater() { return {false, true}; }

    // Operation failed and should not be retried, remove from consumer
    static DashTaskResult failed() { return {false, false}; }

    // Convert from SAI task_process_status to DashTaskResult
    static DashTaskResult fromSaiStatus(task_process_status status)
    {
        if (status == task_need_retry) return retryLater();
        return failed();
    }
};
