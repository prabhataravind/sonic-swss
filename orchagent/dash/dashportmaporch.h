#pragma once

#include "dbconnector.h"
#include "zmqorch.h"
#include "dash_api/outbound_port_map_range.pb.h"
#include "bulker.h"
#include "dashtaskresult.h"

struct DashPortMapBulkContext
{
    std::deque<sai_object_id_t> port_map_oids;
    std::deque<sai_status_t> port_map_statuses;

    DashPortMapBulkContext() {}
    DashPortMapBulkContext(const DashPortMapBulkContext &) = delete;
    DashPortMapBulkContext(DashPortMapBulkContext &) = delete;

    void clear()
    {
        port_map_oids.clear();
        port_map_statuses.clear();
    }
};

struct DashPortMapRangeBulkContext
{
    std::string parent_map_id;
    int start_port;
    int end_port;
    dash::outbound_port_map_range::OutboundPortMapRange metadata;
    std::deque<sai_status_t> port_map_range_statuses;

    DashPortMapRangeBulkContext() {}
    DashPortMapRangeBulkContext(const DashPortMapRangeBulkContext &) = delete;
    DashPortMapRangeBulkContext(DashPortMapRangeBulkContext &) = delete;

    void clear()
    {
        port_map_range_statuses.clear();
    }
};

class DashPortMapOrch : public ZmqOrch
{
public:
    DashPortMapOrch(swss::DBConnector *db, std::vector<std::string> &tables, swss::DBConnector *app_state_db, swss::ZmqServer *zmqServer);
    sai_object_id_t getPortMapOid(const std::string& port_map_name);

private:
    void doTask(ConsumerBase &consumer);
    void doTaskPortMapTable(ConsumerBase &consumer);
    DashTaskResult addPortMap(const std::string &port_map_id, DashPortMapBulkContext &ctxt);
    DashTaskResult addPortMapPost(const std::string &port_map_id, DashPortMapBulkContext &ctxt);
    DashTaskResult removePortMap(const std::string &port_map_id, DashPortMapBulkContext &ctxt);
    DashTaskResult removePortMapPost(const std::string &port_map_id, DashPortMapBulkContext &ctxt);
    void doTaskPortMapRangeTable(ConsumerBase &consumer);
    DashTaskResult addPortMapRange(DashPortMapRangeBulkContext &ctxt);
    DashTaskResult addPortMapRangePost(DashPortMapRangeBulkContext &ctxt);
    DashTaskResult removePortMapRange(DashPortMapRangeBulkContext &ctxt);
    DashTaskResult removePortMapRangePost(DashPortMapRangeBulkContext &ctxt);

    bool parsePortMapRange(const std::string &key, DashPortMapRangeBulkContext &ctxt);

    ObjectBulker<sai_dash_outbound_port_map_api_t> port_map_bulker_;
    EntityBulker<sai_dash_outbound_port_map_api_t> port_map_range_bulker_;

    std::unordered_map<std::string, sai_object_id_t> port_map_table_;
    std::unique_ptr<swss::Table> dash_port_map_result_table_;
    std::unique_ptr<swss::Table> dash_port_map_range_result_table_;
};
