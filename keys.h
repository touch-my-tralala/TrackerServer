#pragma once

#include <QString>

class Keys
{
public:
    using cstr = const QString;

    struct Group
    {
        cstr server    = "SERVER";
        cstr resources = "RESOURCES";
        cstr IPs       = "IPs";
    };

    struct Config
    {
        cstr sattings_path   = "sattings_path";
        cstr port            = "port";
        cstr max_user        = "max_user";
        cstr available_ip    = "available_ip";
        cstr resources       = "resources";
    };

    struct Json
    {
        cstr name      = "name";
        cstr action    = "action";
        cstr resources = "resources";
        cstr goose     = "goose";

        cstr authorization = "authorization";
        cstr status        = "status";
        cstr request       = "request";
        cstr drop_res      = "drop_res";
        cstr take_res      = "take_res";
        cstr hook          = "hook";
        cstr interception  = "interception";

        cstr res_name = "res_name";
        cstr usr_name = "usr_name";
        cstr time = "time";
        cstr info_update = "info_update";
    };

    struct Model
    {
        ///! Common
        cstr fullData = "fullData";

        ///! Resource
        cstr resName = "resName";
        cstr resOwner = "resOwner";
        cstr resTime = "resTime";


        ///! IPs
        cstr ip = "ip";
    };
};
