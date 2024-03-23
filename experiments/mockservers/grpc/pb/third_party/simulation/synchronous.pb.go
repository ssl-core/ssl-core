// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.33.0
// 	protoc        v4.25.3
// source: protocols/third_party/simulation/synchronous.proto

package simulation

import (
	detection "github.com/robocin/ssl-core/protocols/protocols/third_party/detection"
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

// Request from the team to the simulator
type SimulationSyncRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// The simulation step [s] to perform
	SimStep *float32 `protobuf:"fixed32,1,opt,name=sim_step,json=simStep" json:"sim_step,omitempty"`
	// An optional simulator command
	SimulatorCommand *SimulatorCommand `protobuf:"bytes,2,opt,name=simulator_command,json=simulatorCommand" json:"simulator_command,omitempty"`
	// An optional robot control command
	RobotControl *RobotControl `protobuf:"bytes,3,opt,name=robot_control,json=robotControl" json:"robot_control,omitempty"`
}

func (x *SimulationSyncRequest) Reset() {
	*x = SimulationSyncRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_protocols_third_party_simulation_synchronous_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *SimulationSyncRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*SimulationSyncRequest) ProtoMessage() {}

func (x *SimulationSyncRequest) ProtoReflect() protoreflect.Message {
	mi := &file_protocols_third_party_simulation_synchronous_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use SimulationSyncRequest.ProtoReflect.Descriptor instead.
func (*SimulationSyncRequest) Descriptor() ([]byte, []int) {
	return file_protocols_third_party_simulation_synchronous_proto_rawDescGZIP(), []int{0}
}

func (x *SimulationSyncRequest) GetSimStep() float32 {
	if x != nil && x.SimStep != nil {
		return *x.SimStep
	}
	return 0
}

func (x *SimulationSyncRequest) GetSimulatorCommand() *SimulatorCommand {
	if x != nil {
		return x.SimulatorCommand
	}
	return nil
}

func (x *SimulationSyncRequest) GetRobotControl() *RobotControl {
	if x != nil {
		return x.RobotControl
	}
	return nil
}

// Response to last SimulationSyncRequest
type SimulationSyncResponse struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// List of detection frames for all cameras with the state after the
	// simulation step in the request was performed
	Detection []*detection.SSL_DetectionFrame `protobuf:"bytes,1,rep,name=detection" json:"detection,omitempty"`
	// An optional robot control response
	RobotControlResponse *RobotControlResponse `protobuf:"bytes,2,opt,name=robot_control_response,json=robotControlResponse" json:"robot_control_response,omitempty"`
}

func (x *SimulationSyncResponse) Reset() {
	*x = SimulationSyncResponse{}
	if protoimpl.UnsafeEnabled {
		mi := &file_protocols_third_party_simulation_synchronous_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *SimulationSyncResponse) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*SimulationSyncResponse) ProtoMessage() {}

func (x *SimulationSyncResponse) ProtoReflect() protoreflect.Message {
	mi := &file_protocols_third_party_simulation_synchronous_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use SimulationSyncResponse.ProtoReflect.Descriptor instead.
func (*SimulationSyncResponse) Descriptor() ([]byte, []int) {
	return file_protocols_third_party_simulation_synchronous_proto_rawDescGZIP(), []int{1}
}

func (x *SimulationSyncResponse) GetDetection() []*detection.SSL_DetectionFrame {
	if x != nil {
		return x.Detection
	}
	return nil
}

func (x *SimulationSyncResponse) GetRobotControlResponse() *RobotControlResponse {
	if x != nil {
		return x.RobotControlResponse
	}
	return nil
}

var File_protocols_third_party_simulation_synchronous_proto protoreflect.FileDescriptor

var file_protocols_third_party_simulation_synchronous_proto_rawDesc = []byte{
	0x0a, 0x32, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f, 0x74, 0x68, 0x69, 0x72,
	0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2f, 0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x69,
	0x6f, 0x6e, 0x2f, 0x73, 0x79, 0x6e, 0x63, 0x68, 0x72, 0x6f, 0x6e, 0x6f, 0x75, 0x73, 0x2e, 0x70,
	0x72, 0x6f, 0x74, 0x6f, 0x12, 0x20, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2e,
	0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2e, 0x73, 0x69, 0x6d, 0x75,
	0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x1a, 0x29, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c,
	0x73, 0x2f, 0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2f, 0x64, 0x65,
	0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x72, 0x61, 0x77, 0x2e, 0x70, 0x72, 0x6f, 0x74,
	0x6f, 0x1a, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f, 0x74, 0x68, 0x69,
	0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2f, 0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74,
	0x69, 0x6f, 0x6e, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x2e, 0x70, 0x72, 0x6f, 0x74,
	0x6f, 0x1a, 0x34, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f, 0x74, 0x68, 0x69,
	0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2f, 0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74,
	0x69, 0x6f, 0x6e, 0x2f, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f,
	0x6c, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x1a, 0x35, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f,
	0x6c, 0x73, 0x2f, 0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2f, 0x73,
	0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x5f,
	0x66, 0x65, 0x65, 0x64, 0x62, 0x61, 0x63, 0x6b, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x22, 0xe8,
	0x01, 0x0a, 0x15, 0x53, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x53, 0x79, 0x6e,
	0x63, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x12, 0x19, 0x0a, 0x08, 0x73, 0x69, 0x6d, 0x5f,
	0x73, 0x74, 0x65, 0x70, 0x18, 0x01, 0x20, 0x01, 0x28, 0x02, 0x52, 0x07, 0x73, 0x69, 0x6d, 0x53,
	0x74, 0x65, 0x70, 0x12, 0x5f, 0x0a, 0x11, 0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x6f, 0x72,
	0x5f, 0x63, 0x6f, 0x6d, 0x6d, 0x61, 0x6e, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x32,
	0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2e, 0x74, 0x68, 0x69, 0x72, 0x64,
	0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2e, 0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x69, 0x6f,
	0x6e, 0x2e, 0x53, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x6f, 0x72, 0x43, 0x6f, 0x6d, 0x6d, 0x61,
	0x6e, 0x64, 0x52, 0x10, 0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x6f, 0x72, 0x43, 0x6f, 0x6d,
	0x6d, 0x61, 0x6e, 0x64, 0x12, 0x53, 0x0a, 0x0d, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x63, 0x6f,
	0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x18, 0x03, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x2e, 0x2e, 0x70, 0x72,
	0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2e, 0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61,
	0x72, 0x74, 0x79, 0x2e, 0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2e, 0x52,
	0x6f, 0x62, 0x6f, 0x74, 0x43, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x52, 0x0c, 0x72, 0x6f, 0x62,
	0x6f, 0x74, 0x43, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x22, 0xd9, 0x01, 0x0a, 0x16, 0x53, 0x69,
	0x6d, 0x75, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x53, 0x79, 0x6e, 0x63, 0x52, 0x65, 0x73, 0x70,
	0x6f, 0x6e, 0x73, 0x65, 0x12, 0x51, 0x0a, 0x09, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f,
	0x6e, 0x18, 0x01, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x33, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63,
	0x6f, 0x6c, 0x73, 0x2e, 0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2e,
	0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x2e, 0x53, 0x53, 0x4c, 0x5f, 0x44, 0x65,
	0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x46, 0x72, 0x61, 0x6d, 0x65, 0x52, 0x09, 0x64, 0x65,
	0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x12, 0x6c, 0x0a, 0x16, 0x72, 0x6f, 0x62, 0x6f, 0x74,
	0x5f, 0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x5f, 0x72, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73,
	0x65, 0x18, 0x02, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x36, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63,
	0x6f, 0x6c, 0x73, 0x2e, 0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2e,
	0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2e, 0x52, 0x6f, 0x62, 0x6f, 0x74,
	0x43, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x52,
	0x14, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x43, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x52, 0x65, 0x73,
	0x70, 0x6f, 0x6e, 0x73, 0x65, 0x42, 0x48, 0x5a, 0x46, 0x67, 0x69, 0x74, 0x68, 0x75, 0x62, 0x2e,
	0x63, 0x6f, 0x6d, 0x2f, 0x72, 0x6f, 0x62, 0x6f, 0x63, 0x69, 0x6e, 0x2f, 0x73, 0x73, 0x6c, 0x2d,
	0x63, 0x6f, 0x72, 0x65, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f, 0x70,
	0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f, 0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70,
	0x61, 0x72, 0x74, 0x79, 0x2f, 0x73, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e,
}

var (
	file_protocols_third_party_simulation_synchronous_proto_rawDescOnce sync.Once
	file_protocols_third_party_simulation_synchronous_proto_rawDescData = file_protocols_third_party_simulation_synchronous_proto_rawDesc
)

func file_protocols_third_party_simulation_synchronous_proto_rawDescGZIP() []byte {
	file_protocols_third_party_simulation_synchronous_proto_rawDescOnce.Do(func() {
		file_protocols_third_party_simulation_synchronous_proto_rawDescData = protoimpl.X.CompressGZIP(file_protocols_third_party_simulation_synchronous_proto_rawDescData)
	})
	return file_protocols_third_party_simulation_synchronous_proto_rawDescData
}

var file_protocols_third_party_simulation_synchronous_proto_msgTypes = make([]protoimpl.MessageInfo, 2)
var file_protocols_third_party_simulation_synchronous_proto_goTypes = []interface{}{
	(*SimulationSyncRequest)(nil),        // 0: protocols.third_party.simulation.SimulationSyncRequest
	(*SimulationSyncResponse)(nil),       // 1: protocols.third_party.simulation.SimulationSyncResponse
	(*SimulatorCommand)(nil),             // 2: protocols.third_party.simulation.SimulatorCommand
	(*RobotControl)(nil),                 // 3: protocols.third_party.simulation.RobotControl
	(*detection.SSL_DetectionFrame)(nil), // 4: protocols.third_party.detection.SSL_DetectionFrame
	(*RobotControlResponse)(nil),         // 5: protocols.third_party.simulation.RobotControlResponse
}
var file_protocols_third_party_simulation_synchronous_proto_depIdxs = []int32{
	2, // 0: protocols.third_party.simulation.SimulationSyncRequest.simulator_command:type_name -> protocols.third_party.simulation.SimulatorCommand
	3, // 1: protocols.third_party.simulation.SimulationSyncRequest.robot_control:type_name -> protocols.third_party.simulation.RobotControl
	4, // 2: protocols.third_party.simulation.SimulationSyncResponse.detection:type_name -> protocols.third_party.detection.SSL_DetectionFrame
	5, // 3: protocols.third_party.simulation.SimulationSyncResponse.robot_control_response:type_name -> protocols.third_party.simulation.RobotControlResponse
	4, // [4:4] is the sub-list for method output_type
	4, // [4:4] is the sub-list for method input_type
	4, // [4:4] is the sub-list for extension type_name
	4, // [4:4] is the sub-list for extension extendee
	0, // [0:4] is the sub-list for field type_name
}

func init() { file_protocols_third_party_simulation_synchronous_proto_init() }
func file_protocols_third_party_simulation_synchronous_proto_init() {
	if File_protocols_third_party_simulation_synchronous_proto != nil {
		return
	}
	file_protocols_third_party_simulation_control_proto_init()
	file_protocols_third_party_simulation_robot_control_proto_init()
	file_protocols_third_party_simulation_robot_feedback_proto_init()
	if !protoimpl.UnsafeEnabled {
		file_protocols_third_party_simulation_synchronous_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*SimulationSyncRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_protocols_third_party_simulation_synchronous_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*SimulationSyncResponse); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_protocols_third_party_simulation_synchronous_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   2,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_protocols_third_party_simulation_synchronous_proto_goTypes,
		DependencyIndexes: file_protocols_third_party_simulation_synchronous_proto_depIdxs,
		MessageInfos:      file_protocols_third_party_simulation_synchronous_proto_msgTypes,
	}.Build()
	File_protocols_third_party_simulation_synchronous_proto = out.File
	file_protocols_third_party_simulation_synchronous_proto_rawDesc = nil
	file_protocols_third_party_simulation_synchronous_proto_goTypes = nil
	file_protocols_third_party_simulation_synchronous_proto_depIdxs = nil
}
