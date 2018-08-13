/*
Tencent is pleased to support the open source community by making
PhxRPC available.
Copyright (C) 2016 THL A29 Limited, a Tencent company.
All rights reserved.

Licensed under the BSD 3-Clause License (the "License"); you may
not use this file except in compliance with the License. You may
obtain a copy of the License at

https://opensource.org/licenses/BSD-3-Clause

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied. See the License for the specific language governing
permissions and limitations under the License.

See the AUTHORS file for names of contributors.
*/

#pragma once

#include <vector>
#include <string>

#include "phxrpc/msg.h"


namespace phxqueue_phxrpc {

namespace mqttbroker {


class MqttMessage : virtual public phxrpc::BaseMessage {
  public:
    enum class ControlPacketType {
        FAKE_NONE = 0,
        CONNECT = 1,
        CONNACK,
        PUBLISH,
        PUBACK,
        PUBREC,
        PUBREL,
        PUBCOMP,
        SUBSCRIBE,
        SUBACK,
        UNSUBSCRIBE,
        UNSUBACK,
        PINGREQ,
        PINGRESP,
        DISCONNECT,
        FAKE_MAX,
    };

    struct FixedHeader {
        ControlPacketType control_packet_type{ControlPacketType::FAKE_NONE};
        bool dup{0};
        uint32_t qos{0};
        bool retain{0};
    };

    static const char SampleFixedHeader[];

    static int EncodeUint16(std::string &dest, const uint16_t src);
    static int EncodeUint16(char *const dest, const size_t dest_size,
                            const uint16_t src);
    static int EncodeUnicode(std::string &dest, const std::string &src);
    static int EncodeUnicode(char *const dest, const size_t dest_size,
                             const std::string &src);

    static phxrpc::ReturnCode SendChar(std::ostringstream &out_stream, const char &content);
    static phxrpc::ReturnCode RecvChar(std::istringstream &in_stream, char &content);
    static phxrpc::ReturnCode SendUint16(std::ostringstream &out_stream,
                                 const uint16_t content);
    static phxrpc::ReturnCode RecvUint16(std::istringstream &in_stream,
                                 uint16_t &content);
    static phxrpc::ReturnCode SendChars(std::ostringstream &out_stream,
                                const char *const content,
                                const int content_length);
    static phxrpc::ReturnCode RecvChars(std::istringstream &in_stream,
                                char *const content,
                                const int content_length);
    static phxrpc::ReturnCode SendUnicode(std::ostringstream &out_stream,
                                  const std::string &content);
    static phxrpc::ReturnCode RecvUnicode(std::istringstream &in_stream,
                                  std::string &content);

    static phxrpc::ReturnCode SendChar(phxrpc::BaseTcpStream &out_stream, const char &content);
    static phxrpc::ReturnCode RecvChar(phxrpc::BaseTcpStream &in_stream, char &content);
    static phxrpc::ReturnCode SendUint16(phxrpc::BaseTcpStream &out_stream,
                                 const uint16_t content);
    static phxrpc::ReturnCode RecvUint16(phxrpc::BaseTcpStream &in_stream, uint16_t &content);
    static phxrpc::ReturnCode SendChars(phxrpc::BaseTcpStream &out_stream,
                                const char *const content,
                                const int content_length);
    static phxrpc::ReturnCode RecvChars(phxrpc::BaseTcpStream &in_stream,
                                char *const content,
                                const int content_length);
    static phxrpc::ReturnCode SendUnicode(phxrpc::BaseTcpStream &out_stream,
                                  const std::string &content);
    static phxrpc::ReturnCode RecvUnicode(phxrpc::BaseTcpStream &in_stream,
                                  std::string &content);

    static phxrpc::ReturnCode SendChar(const int fd, const char &content);
    static phxrpc::ReturnCode RecvChar(const int fd, char &content);
    static phxrpc::ReturnCode SendUint16(const int fd, const uint16_t content);
    static phxrpc::ReturnCode RecvUint16(const int fd, uint16_t &content);
    static phxrpc::ReturnCode SendChars(const int fd, const char *const content,
                                const int content_length);
    static phxrpc::ReturnCode RecvChars(const int fd, char *const content,
                                const int content_length);
    static phxrpc::ReturnCode SendUnicode(const int fd, const std::string &content);
    static phxrpc::ReturnCode RecvUnicode(const int fd, std::string &content);

    static uint8_t EncodeFixedHeader(const FixedHeader &fixed_header);
    static FixedHeader DecodeFixedHeader(const uint8_t fixed_header_byte);

    // control packet type and flags
    static phxrpc::ReturnCode SendFixedHeaderAndRemainingBuffer(
            phxrpc::BaseTcpStream &out_stream,
            const FixedHeader &fixed_header,
            const std::string &remaining_buffer);
    static phxrpc::ReturnCode RecvFixedHeaderAndRemainingBuffer(
            phxrpc::BaseTcpStream &in_stream,
            FixedHeader &fixed_header,
            std::string &remaining_buffer);

    static phxrpc::ReturnCode SendFixedHeaderAndRemainingBuffer(
            const int fd, const FixedHeader &fixed_header,
            const std::string &remaining_buffer);
    static phxrpc::ReturnCode RecvFixedHeaderAndRemainingBuffer(
            const int fd, FixedHeader &fixed_header,
            std::string &remaining_buffer);

    // remaining length
    static phxrpc::ReturnCode SendRemainingLength(phxrpc::BaseTcpStream &out_stream,
                                          const int remaining_length);
    static phxrpc::ReturnCode RecvRemainingLength(phxrpc::BaseTcpStream &in_stream,
                                          int &remaining_length);

    static phxrpc::ReturnCode SendRemainingLength(const int fd, const int remaining_length);
    static phxrpc::ReturnCode RecvRemainingLength(const int fd, int &remaining_length);

    MqttMessage();
    virtual ~MqttMessage() override;

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const = 0;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) = 0;

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const = 0;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) = 0;

    virtual phxrpc::ReturnCode Send(phxrpc::BaseTcpStream &socket) const override;

    phxrpc::ReturnCode SendRemaining(std::ostringstream &out_stream) const;
    phxrpc::ReturnCode RecvRemaining(std::istringstream &in_stream);

    // packet identifier
    phxrpc::ReturnCode SendPacketIdentifier(std::ostringstream &out_stream) const;
    phxrpc::ReturnCode RecvPacketIdentifier(std::istringstream &in_stream);

    FixedHeader fixed_header() const { return fixed_header_; }

    void set_fixed_header(const FixedHeader fixed_header) {
        fixed_header_ = fixed_header;
    }

    FixedHeader &mutable_fixed_header() {
        return fixed_header_;
    }

    uint16_t packet_identifier() const { return packet_identifier_; }

    void set_packet_identifier(const uint16_t packet_identifier) {
        packet_identifier_ = packet_identifier;
    }

    int remaining_length() const { return remaining_length_; }

    void set_remaining_length(const int remaining_length) {
        remaining_length_ = remaining_length;
    }

  private:
    FixedHeader fixed_header_;
    uint16_t packet_identifier_{0x0};
    int remaining_length_{0};
};


class MqttRequest : public virtual MqttMessage, public phxrpc::BaseRequest {
  public:
    MqttRequest() = default;
    virtual ~MqttRequest() = default;

  private:
};


class MqttResponse : public virtual MqttMessage, public phxrpc::BaseResponse {
  public:
    MqttResponse() = default;
    virtual ~MqttResponse() = default;

    virtual void SetPhxRpcResult(const int result) override {}
    virtual void DispatchErr() override {}

    virtual phxrpc::ReturnCode ModifyResp(const bool keep_alive, const std::string &version) override;

  private:
};


class MqttFakeResponse final : public MqttResponse {
  public:
    MqttFakeResponse();
    virtual ~MqttFakeResponse() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override {
        return phxrpc::ReturnCode::ERROR_UNIMPLEMENT;
    }

    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override {
        return phxrpc::ReturnCode::ERROR_UNIMPLEMENT;
    }

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }
};


class MqttConnect final : public MqttRequest {
  public:
    MqttConnect();
    virtual ~MqttConnect() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override;

    bool clean_session() const { return clean_session_; }
    void set_clean_session(const bool clean_session) {
        clean_session_ = clean_session;
    }

    uint16_t keep_alive() const { return keep_alive_; }
    void set_keep_alive(const uint16_t keep_alive) {
        keep_alive_ = keep_alive;
    }

    const std::string &client_identifier() const { return client_identifier_; }
    void set_client_identifier(const std::string &client_identifier) {
        client_identifier_ = client_identifier;
    }

    // read only
    const std::string &proto_name() const { return proto_name_; }

    // read only
    uint8_t proto_level() const { return proto_level_; }

  private:
    std::string client_identifier_;
    std::string proto_name_{"MQTT"};
    uint8_t proto_level_{4};
    bool clean_session_{false};
    uint16_t keep_alive_{0};
    bool user_name_flag_{false};
    bool password_flag_{false};
    std::string user_name_;
    std::string password_;
    bool will_flag_{false};
    uint32_t will_qos_{0};
    bool will_retain_{false};
    std::string will_topic_;
    std::string will_message_;
};


class MqttConnack final : public MqttResponse {
  public:
    MqttConnack();
    virtual ~MqttConnack() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }

    bool session_present() const { return session_present_; }
    void set_session_present(const bool session_present) {
        session_present_ = session_present;
    }

    char connect_return_code() const { return connect_return_code_; }
    void set_connect_return_code(const char connect_return_code) {
        connect_return_code_ = connect_return_code;
    }

  private:
    bool session_present_{0};
    char connect_return_code_{0};
};


class MqttPublish final : public virtual MqttRequest, public virtual MqttResponse {
  public:
    MqttPublish();
    virtual ~MqttPublish() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override;

    const std::string &topic_name() const { return topic_name_; }
    void set_topic_name(const std::string &topic_name) {
        topic_name_ = topic_name;
    }

  private:
    std::string topic_name_;
};


class MqttPuback final : public virtual MqttRequest, public virtual MqttResponse {
  public:
    MqttPuback();
    virtual ~MqttPuback() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;
    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }
};


class MqttPubrec final : public virtual MqttRequest, public virtual MqttResponse {
  public:
    MqttPubrec();
    virtual ~MqttPubrec() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override;
};


class MqttPubrel final : public virtual MqttRequest, public virtual MqttResponse {
  public:
    MqttPubrel();
    virtual ~MqttPubrel() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override;
};


class MqttPubcomp final : public virtual MqttRequest, public virtual MqttResponse {
  public:
    MqttPubcomp();
    virtual ~MqttPubcomp() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override;
};


class MqttSubscribe final : public MqttRequest {
  public:
    MqttSubscribe();
    virtual ~MqttSubscribe() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;
    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override;

    const std::vector<std::string> &topic_filters() const {
        return topic_filters_;
    }
    void set_topic_filters(const std::vector<std::string> &topic_filters) {
        topic_filters_ = topic_filters;
    }

  private:
    std::vector<std::string> topic_filters_;
    std::vector<uint32_t> qoss_;
};


class MqttSuback final : public MqttResponse {
  public:
    MqttSuback();
    virtual ~MqttSuback() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;
    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override;

    const std::string &return_codes() const { return return_codes_; }
    void set_return_codes(const std::string &return_codes) {
        return_codes_ = return_codes;
    }

  private:
    std::string return_codes_;
};


class MqttUnsubscribe final : public MqttRequest {
  public:
    MqttUnsubscribe();
    virtual ~MqttUnsubscribe() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;
    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override;

    const std::vector<std::string> &topic_filters() const {
        return topic_filters_;
    }
    void set_topic_filters(const std::vector<std::string> &topic_filters) {
        topic_filters_ = topic_filters;
    }

  private:
    std::vector<std::string> topic_filters_;
};


class MqttUnsuback final : public MqttResponse {
  public:
    MqttUnsuback();
    virtual ~MqttUnsuback() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override;
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override;

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }
};


class MqttPingreq final : public MqttRequest {
  public:
    MqttPingreq();
    virtual ~MqttPingreq() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }
};


class MqttPingresp final : public MqttResponse {
  public:
    MqttPingresp();
    virtual ~MqttPingresp() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }
};


class MqttDisconnect final : public MqttRequest {
  public:
    MqttDisconnect();
    virtual ~MqttDisconnect() = default;

    virtual phxrpc::ReturnCode ToPb(google::protobuf::Message *const message) const override;
    virtual phxrpc::ReturnCode FromPb(const google::protobuf::Message &message) override;

    virtual phxrpc::BaseResponse *GenResponse() const override;
    virtual int IsKeepAlive() const override { return 1; };

    virtual phxrpc::ReturnCode SendVariableHeader(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvVariableHeader(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }

    virtual phxrpc::ReturnCode SendPayload(std::ostringstream &out_stream) const override {
        return phxrpc::ReturnCode::OK;
    }
    virtual phxrpc::ReturnCode RecvPayload(std::istringstream &in_stream) override {
        return phxrpc::ReturnCode::OK;
    }
};


}  // namespace mqttbroker

}  // namespace phxqueue_phxrpc
