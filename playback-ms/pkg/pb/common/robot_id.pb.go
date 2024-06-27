// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.28.1
// 	protoc        v3.21.12
// source: protocols/common/robot_id.proto

package common

import (
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

// Color is the enum describing the color of the robot.
type RobotId_Color int32

const (
	RobotId_COLOR_UNSPECIFIED RobotId_Color = 0
	RobotId_COLOR_YELLOW      RobotId_Color = 1
	RobotId_COLOR_BLUE        RobotId_Color = 2
)

// Enum value maps for RobotId_Color.
var (
	RobotId_Color_name = map[int32]string{
		0: "COLOR_UNSPECIFIED",
		1: "COLOR_YELLOW",
		2: "COLOR_BLUE",
	}
	RobotId_Color_value = map[string]int32{
		"COLOR_UNSPECIFIED": 0,
		"COLOR_YELLOW":      1,
		"COLOR_BLUE":        2,
	}
)

func (x RobotId_Color) Enum() *RobotId_Color {
	p := new(RobotId_Color)
	*p = x
	return p
}

func (x RobotId_Color) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (RobotId_Color) Descriptor() protoreflect.EnumDescriptor {
	return file_protocols_common_robot_id_proto_enumTypes[0].Descriptor()
}

func (RobotId_Color) Type() protoreflect.EnumType {
	return &file_protocols_common_robot_id_proto_enumTypes[0]
}

func (x RobotId_Color) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use RobotId_Color.Descriptor instead.
func (RobotId_Color) EnumDescriptor() ([]byte, []int) {
	return file_protocols_common_robot_id_proto_rawDescGZIP(), []int{0, 0}
}

// RobotId is the message describing the id of a robot.
type RobotId struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// The color of the robot.
	Color RobotId_Color `protobuf:"varint,1,opt,name=color,proto3,enum=protocols.common.RobotId_Color" json:"color,omitempty"`
	// The number of the robot.
	Number uint32 `protobuf:"varint,2,opt,name=number,proto3" json:"number,omitempty"`
}

func (x *RobotId) Reset() {
	*x = RobotId{}
	if protoimpl.UnsafeEnabled {
		mi := &file_protocols_common_robot_id_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *RobotId) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*RobotId) ProtoMessage() {}

func (x *RobotId) ProtoReflect() protoreflect.Message {
	mi := &file_protocols_common_robot_id_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use RobotId.ProtoReflect.Descriptor instead.
func (*RobotId) Descriptor() ([]byte, []int) {
	return file_protocols_common_robot_id_proto_rawDescGZIP(), []int{0}
}

func (x *RobotId) GetColor() RobotId_Color {
	if x != nil {
		return x.Color
	}
	return RobotId_COLOR_UNSPECIFIED
}

func (x *RobotId) GetNumber() uint32 {
	if x != nil {
		return x.Number
	}
	return 0
}

var File_protocols_common_robot_id_proto protoreflect.FileDescriptor

var file_protocols_common_robot_id_proto_rawDesc = []byte{
	0x0a, 0x1f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f, 0x63, 0x6f, 0x6d, 0x6d,
	0x6f, 0x6e, 0x2f, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x69, 0x64, 0x2e, 0x70, 0x72, 0x6f, 0x74,
	0x6f, 0x12, 0x10, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2e, 0x63, 0x6f, 0x6d,
	0x6d, 0x6f, 0x6e, 0x22, 0x9a, 0x01, 0x0a, 0x07, 0x52, 0x6f, 0x62, 0x6f, 0x74, 0x49, 0x64, 0x12,
	0x35, 0x0a, 0x05, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0e, 0x32, 0x1f,
	0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2e, 0x63, 0x6f, 0x6d, 0x6d, 0x6f,
	0x6e, 0x2e, 0x52, 0x6f, 0x62, 0x6f, 0x74, 0x49, 0x64, 0x2e, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x52,
	0x05, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x12, 0x16, 0x0a, 0x06, 0x6e, 0x75, 0x6d, 0x62, 0x65, 0x72,
	0x18, 0x02, 0x20, 0x01, 0x28, 0x0d, 0x52, 0x06, 0x6e, 0x75, 0x6d, 0x62, 0x65, 0x72, 0x22, 0x40,
	0x0a, 0x05, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x12, 0x15, 0x0a, 0x11, 0x43, 0x4f, 0x4c, 0x4f, 0x52,
	0x5f, 0x55, 0x4e, 0x53, 0x50, 0x45, 0x43, 0x49, 0x46, 0x49, 0x45, 0x44, 0x10, 0x00, 0x12, 0x10,
	0x0a, 0x0c, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x5f, 0x59, 0x45, 0x4c, 0x4c, 0x4f, 0x57, 0x10, 0x01,
	0x12, 0x0e, 0x0a, 0x0a, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x5f, 0x42, 0x4c, 0x55, 0x45, 0x10, 0x02,
	0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_protocols_common_robot_id_proto_rawDescOnce sync.Once
	file_protocols_common_robot_id_proto_rawDescData = file_protocols_common_robot_id_proto_rawDesc
)

func file_protocols_common_robot_id_proto_rawDescGZIP() []byte {
	file_protocols_common_robot_id_proto_rawDescOnce.Do(func() {
		file_protocols_common_robot_id_proto_rawDescData = protoimpl.X.CompressGZIP(file_protocols_common_robot_id_proto_rawDescData)
	})
	return file_protocols_common_robot_id_proto_rawDescData
}

var file_protocols_common_robot_id_proto_enumTypes = make([]protoimpl.EnumInfo, 1)
var file_protocols_common_robot_id_proto_msgTypes = make([]protoimpl.MessageInfo, 1)
var file_protocols_common_robot_id_proto_goTypes = []interface{}{
	(RobotId_Color)(0), // 0: protocols.common.RobotId.Color
	(*RobotId)(nil),    // 1: protocols.common.RobotId
}
var file_protocols_common_robot_id_proto_depIdxs = []int32{
	0, // 0: protocols.common.RobotId.color:type_name -> protocols.common.RobotId.Color
	1, // [1:1] is the sub-list for method output_type
	1, // [1:1] is the sub-list for method input_type
	1, // [1:1] is the sub-list for extension type_name
	1, // [1:1] is the sub-list for extension extendee
	0, // [0:1] is the sub-list for field type_name
}

func init() { file_protocols_common_robot_id_proto_init() }
func file_protocols_common_robot_id_proto_init() {
	if File_protocols_common_robot_id_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_protocols_common_robot_id_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*RobotId); i {
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
			RawDescriptor: file_protocols_common_robot_id_proto_rawDesc,
			NumEnums:      1,
			NumMessages:   1,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_protocols_common_robot_id_proto_goTypes,
		DependencyIndexes: file_protocols_common_robot_id_proto_depIdxs,
		EnumInfos:         file_protocols_common_robot_id_proto_enumTypes,
		MessageInfos:      file_protocols_common_robot_id_proto_msgTypes,
	}.Build()
	File_protocols_common_robot_id_proto = out.File
	file_protocols_common_robot_id_proto_rawDesc = nil
	file_protocols_common_robot_id_proto_goTypes = nil
	file_protocols_common_robot_id_proto_depIdxs = nil
}
