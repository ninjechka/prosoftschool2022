#include "tests.h"
#include "devicemock.h"
#include "devicemonitoringserver.h"
#include "test_runner.h"
#include <servermock/clientconnectionmock.h>
#include <servermock/connectionservermock.h>
#include <servermock/taskqueue.h>
#include "vector"

void monitoringServerTest1()
{
    TaskQueue taskQueue;
    DeviceMock device(new ClientConnectionMock(taskQueue));
    DeviceMonitoringServer server(new ConnectionServerMock(taskQueue));

    const uint64_t deviceId = 111;
    const uint64_t serverId = 11;
    ASSERT(device.bind(deviceId));
    ASSERT(server.listen(serverId));
    ASSERT(device.connectToServer(serverId));
    std::vector<uint8_t> mets = {31, 2};
    std::vector<Phase> phases;
    for (uint64_t i = 1; i < mets.size() + 1; i++)
    {
        phases.push_back({i, mets[i - 1]});
    }
    DeviceWorkSchedule const deviceWorkSchedule = {deviceId, phases};
    server.setDeviceWorkSchedule(deviceWorkSchedule);
    device.setMeterages({32, 2});
    std::vector<std::string> expected = {"1 -1", "1 0"};
    while (taskQueue.processTask())
        ;
    device.startMeterageSending();
    for (int i = 0; i < mets.size()*2 + 2; i++)
        taskQueue.processTask();
    std::vector<std::string> responses = device.getResponses();
    if (responses.size() != expected.size())
        ASSERT(false);
    for (int i = 0; i < responses.size(); i++)
    {
        ASSERT_EQUAL(responses[i], expected[i]);
    }
}

void monitoringServerTestErrorObsolete()
{
    TaskQueue taskQueue;
    DeviceMock device(new ClientConnectionMock(taskQueue));
    DeviceMonitoringServer server(new ConnectionServerMock(taskQueue));

    const uint64_t deviceId = 111;
    const uint64_t serverId = 11;
    ASSERT(device.bind(deviceId));
    ASSERT(server.listen(serverId));
    ASSERT(device.connectToServer(serverId));
    std::vector<uint8_t> mets = {31};
    std::vector<Phase> phases;
    for (uint64_t i = 1; i < mets.size() + 1; i++)
    {
        phases.push_back({i, mets[i - 1]});
    }
    DeviceWorkSchedule const deviceWorkSchedule = {deviceId, phases};
    server.setDeviceWorkSchedule(deviceWorkSchedule);
    device.setMeterages({32, 1});
    std::vector<std::string> expected = {"1 -1", "2 102"};
    while (taskQueue.processTask())
        ;
    device.startMeterageSending();
    for (int i = 0; i < mets.size()*2 + 2; i++)
        taskQueue.processTask();
    std::vector<std::string> responses = device.getResponses();
    if (responses.size() != expected.size())
    ASSERT(false);
    for (int i = 0; i < responses.size(); i++)
    {
        ASSERT_EQUAL(responses[i], expected[i]);
    }
}

void monitoringServerTestErrorNoSchedule()
{
    TaskQueue taskQueue;
    DeviceMock device(new ClientConnectionMock(taskQueue));
    DeviceMonitoringServer server(new ConnectionServerMock(taskQueue));

    const uint64_t deviceId = 111;
    const uint64_t serverId = 11;
    ASSERT(device.bind(deviceId));
    ASSERT(server.listen(serverId));
    ASSERT(device.connectToServer(serverId));
    device.setMeterages({31});
    std::vector<std::string> expected = {"2 101"};
    while (taskQueue.processTask())
        ;
    device.startMeterageSending();
    for (int i = 0; i < 2; i++)
        taskQueue.processTask();
    std::vector<std::string> responses = device.getResponses();
    if (responses.size() != expected.size())
    ASSERT(false);
    for (int i = 0; i < responses.size(); i++)
    {
        ASSERT_EQUAL(responses[i], expected[i]);
    }
}

void monitoringServerTestErrorNoTimestamp()
{
    TaskQueue taskQueue;
    DeviceMock device(new ClientConnectionMock(taskQueue));
    DeviceMonitoringServer server(new ConnectionServerMock(taskQueue));

    const uint64_t deviceId = 111;
    const uint64_t serverId = 11;
    ASSERT(device.bind(deviceId));
    ASSERT(server.listen(serverId));
    ASSERT(device.connectToServer(serverId));
    std::vector<uint8_t> mets = {31};
    std::vector<Phase> phases;
    for (uint64_t i = 1; i < mets.size() + 1; i++)
    {
        phases.push_back({i, mets[i - 1]});
    }
    DeviceWorkSchedule const deviceWorkSchedule = {deviceId, phases};
    server.setDeviceWorkSchedule(deviceWorkSchedule);
    device.setMeterages({32, 1});
    std::vector<std::string> expected = {"1 -1", "2 102"};
    while (taskQueue.processTask())
        ;
    device.startMeterageSending();
    for (int i = 0; i < mets.size()*2 + 2; i++)
        taskQueue.processTask();
    std::vector<std::string> responses = device.getResponses();
    if (responses.size() != expected.size())
    ASSERT(false);
    for (int i = 0; i < responses.size(); i++)
    {
        ASSERT_EQUAL(responses[i], expected[i]);
    }
}

void monitoringServerTestNegativeValue()
{
    TaskQueue taskQueue;
    DeviceMock device(new ClientConnectionMock(taskQueue));
    DeviceMonitoringServer server(new ConnectionServerMock(taskQueue));

    const uint64_t deviceId = 111;
    const uint64_t serverId = 11;
    ASSERT(device.bind(deviceId));
    ASSERT(server.listen(serverId));
    ASSERT(device.connectToServer(serverId));
    std::vector<uint8_t> mets = {3};
    std::vector<Phase> phases;
    for (uint64_t i = 1; i < mets.size() + 1; i++)
    {
        phases.push_back({i, mets[i - 1]});
    }
    DeviceWorkSchedule const deviceWorkSchedule = {deviceId, phases};
    server.setDeviceWorkSchedule(deviceWorkSchedule);
    device.setMeterages({5});
    std::vector<std::string> expected = {"1 -2"};
    while (taskQueue.processTask())
        ;
    device.startMeterageSending();
    for (int i = 0; i < mets.size()*2 + 2; i++)
        taskQueue.processTask();
    std::vector<std::string> responses = device.getResponses();
    if (responses.size() != expected.size())
    ASSERT(false);
    for (int i = 0; i < responses.size(); i++)
    {
        ASSERT_EQUAL(responses[i], expected[i]);
    }
}

void monitoringServerTestComplex()
{
    TaskQueue taskQueue;
    DeviceMock device(new ClientConnectionMock(taskQueue));
    DeviceMonitoringServer server(new ConnectionServerMock(taskQueue));

    const uint64_t deviceId = 111;
    const uint64_t serverId = 11;
    ASSERT(device.bind(deviceId));
    ASSERT(server.listen(serverId));
    ASSERT(device.connectToServer(serverId));
    std::vector<uint8_t> mets = {31, 2, 4, 5, 6};
    std::vector<Phase> phases;
    for (uint64_t i = 1; i < mets.size() + 1; i++)
    {
        phases.push_back({i, mets[i - 1]});
    }
    DeviceWorkSchedule const deviceWorkSchedule = {deviceId, phases};
    server.setDeviceWorkSchedule(deviceWorkSchedule);
    device.setMeterages({31, 2, 8, 3, 7, 7});
    std::vector<std::string> expected = {"1 0", "1 0", "1 -4", "1 2", "1 -1", "2 102"};
    while (taskQueue.processTask())
        ;
    device.startMeterageSending();
    for (int i = 0; i < mets.size()*2 + 2; i++)
        taskQueue.processTask();
    std::vector<std::string> responses = device.getResponses();
    if (responses.size() != expected.size())
        ASSERT(false);
    for (int i = 0; i < responses.size(); i++)
    {
        ASSERT_EQUAL(responses[i], expected[i]);
    }
}

void monitoringServerTestStandardDeviation()
{
    TaskQueue taskQueue;
    DeviceMock device(new ClientConnectionMock(taskQueue));
    DeviceMonitoringServer server(new ConnectionServerMock(taskQueue));

    const uint64_t deviceId = 111;
    const uint64_t serverId = 11;
    ASSERT(device.bind(deviceId));
    ASSERT(server.listen(serverId));
    ASSERT(device.connectToServer(serverId));
    std::vector<uint8_t> mets = {31, 2, 4, 5, 6};
    std::vector<Phase> phases;
    for (uint64_t i = 1; i < mets.size() + 1; i++)
    {
        phases.push_back({i, mets[i - 1]});
    }
    DeviceWorkSchedule const deviceWorkSchedule = {deviceId, phases};
    server.setDeviceWorkSchedule(deviceWorkSchedule);
    device.setMeterages({31, 2, 8, 3, 7});
    std::vector<std::string> expected = {"1 0", "1 0", "1 -4", "1 2", "1 -1"};
    while (taskQueue.processTask())
        ;
    device.startMeterageSending();
    for (int i = 0; i < mets.size()*2 + 2; i++)
        taskQueue.processTask();
    std::vector<std::string> responses = device.getResponses();
    if (responses.size() != expected.size())
    ASSERT(false);
    for (int i = 0; i < responses.size(); i++)
    {
        ASSERT_EQUAL(responses[i], expected[i]);
    }
    double sd = server.getStandardDeviation(deviceId);
    ASSERT_EQUAL(sd, 1.96);
}
