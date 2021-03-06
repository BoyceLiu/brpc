// Copyright (c) 2017 Baidu, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// A thrift client sending requests to server every 1 second.

#include <gflags/gflags.h>
#include "gen-cpp/EchoService.h"
#include "gen-cpp/echo_types.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include <butil/logging.h>

DEFINE_string(server, "0.0.0.0", "IP Address of server");
DEFINE_int32(port, 8019, "Port of server");

int main(int argc, char **argv) {

    // Parse gflags. We recommend you to use gflags as well.
    google::ParseCommandLineFlags(&argc, &argv, true);

    boost::shared_ptr<apache::thrift::transport::TSocket> socket(
        new apache::thrift::transport::TSocket(FLAGS_server, FLAGS_port));
    boost::shared_ptr<apache::thrift::transport::TTransport> transport(
        new apache::thrift::transport::TFramedTransport(socket));
    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol(
        new apache::thrift::protocol::TBinaryProtocol(transport));

    example::EchoServiceClient client(protocol);
    transport->open();

    example::EchoRequest req;
    req.data = "hello";

    example::EchoResponse res;

    while (1) {
        client.Echo(res, req);

        LOG(INFO) << "Req: " << req.data
                  << " Res: " << res.data;
        sleep(1);
    }
    transport->close();

    return 0;
}
