// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.28.1
// 	protoc        v3.21.12
// source: protocols/third_party/detection/raw_wrapper.proto

package detection

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

type SSL_WrapperPacket struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Detection *SSL_DetectionFrame `protobuf:"bytes,1,opt,name=detection" json:"detection,omitempty"`
	Geometry  *SSL_GeometryData   `protobuf:"bytes,2,opt,name=geometry" json:"geometry,omitempty"`
}

func (x *SSL_WrapperPacket) Reset() {
	*x = SSL_WrapperPacket{}
	if protoimpl.UnsafeEnabled {
		mi := &file_protocols_third_party_detection_raw_wrapper_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *SSL_WrapperPacket) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*SSL_WrapperPacket) ProtoMessage() {}

func (x *SSL_WrapperPacket) ProtoReflect() protoreflect.Message {
	mi := &file_protocols_third_party_detection_raw_wrapper_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use SSL_WrapperPacket.ProtoReflect.Descriptor instead.
func (*SSL_WrapperPacket) Descriptor() ([]byte, []int) {
	return file_protocols_third_party_detection_raw_wrapper_proto_rawDescGZIP(), []int{0}
}

func (x *SSL_WrapperPacket) GetDetection() *SSL_DetectionFrame {
	if x != nil {
		return x.Detection
	}
	return nil
}

func (x *SSL_WrapperPacket) GetGeometry() *SSL_GeometryData {
	if x != nil {
		return x.Geometry
	}
	return nil
}

var File_protocols_third_party_detection_raw_wrapper_proto protoreflect.FileDescriptor

var file_protocols_third_party_detection_raw_wrapper_proto_rawDesc = []byte{
	0x0a, 0x31, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f, 0x74, 0x68, 0x69, 0x72,
	0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2f, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f,
	0x6e, 0x2f, 0x72, 0x61, 0x77, 0x5f, 0x77, 0x72, 0x61, 0x70, 0x70, 0x65, 0x72, 0x2e, 0x70, 0x72,
	0x6f, 0x74, 0x6f, 0x12, 0x1f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2e, 0x74,
	0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2e, 0x64, 0x65, 0x74, 0x65, 0x63,
	0x74, 0x69, 0x6f, 0x6e, 0x1a, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f,
	0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2f, 0x64, 0x65, 0x74, 0x65,
	0x63, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x67, 0x65, 0x6f, 0x6d, 0x65, 0x74, 0x72, 0x79, 0x2e, 0x70,
	0x72, 0x6f, 0x74, 0x6f, 0x1a, 0x29, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2f,
	0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79, 0x2f, 0x64, 0x65, 0x74, 0x65,
	0x63, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x72, 0x61, 0x77, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x22,
	0xb5, 0x01, 0x0a, 0x11, 0x53, 0x53, 0x4c, 0x5f, 0x57, 0x72, 0x61, 0x70, 0x70, 0x65, 0x72, 0x50,
	0x61, 0x63, 0x6b, 0x65, 0x74, 0x12, 0x51, 0x0a, 0x09, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x69,
	0x6f, 0x6e, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x33, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x63, 0x6f, 0x6c, 0x73, 0x2e, 0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72, 0x74, 0x79,
	0x2e, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x2e, 0x53, 0x53, 0x4c, 0x5f, 0x44,
	0x65, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x46, 0x72, 0x61, 0x6d, 0x65, 0x52, 0x09, 0x64,
	0x65, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x12, 0x4d, 0x0a, 0x08, 0x67, 0x65, 0x6f, 0x6d,
	0x65, 0x74, 0x72, 0x79, 0x18, 0x02, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x31, 0x2e, 0x70, 0x72, 0x6f,
	0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x73, 0x2e, 0x74, 0x68, 0x69, 0x72, 0x64, 0x5f, 0x70, 0x61, 0x72,
	0x74, 0x79, 0x2e, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x2e, 0x53, 0x53, 0x4c,
	0x5f, 0x47, 0x65, 0x6f, 0x6d, 0x65, 0x74, 0x72, 0x79, 0x44, 0x61, 0x74, 0x61, 0x52, 0x08, 0x67,
	0x65, 0x6f, 0x6d, 0x65, 0x74, 0x72, 0x79,
}

var (
	file_protocols_third_party_detection_raw_wrapper_proto_rawDescOnce sync.Once
	file_protocols_third_party_detection_raw_wrapper_proto_rawDescData = file_protocols_third_party_detection_raw_wrapper_proto_rawDesc
)

func file_protocols_third_party_detection_raw_wrapper_proto_rawDescGZIP() []byte {
	file_protocols_third_party_detection_raw_wrapper_proto_rawDescOnce.Do(func() {
		file_protocols_third_party_detection_raw_wrapper_proto_rawDescData = protoimpl.X.CompressGZIP(file_protocols_third_party_detection_raw_wrapper_proto_rawDescData)
	})
	return file_protocols_third_party_detection_raw_wrapper_proto_rawDescData
}

var file_protocols_third_party_detection_raw_wrapper_proto_msgTypes = make([]protoimpl.MessageInfo, 1)
var file_protocols_third_party_detection_raw_wrapper_proto_goTypes = []interface{}{
	(*SSL_WrapperPacket)(nil),  // 0: protocols.third_party.detection.SSL_WrapperPacket
	(*SSL_DetectionFrame)(nil), // 1: protocols.third_party.detection.SSL_DetectionFrame
	(*SSL_GeometryData)(nil),   // 2: protocols.third_party.detection.SSL_GeometryData
}
var file_protocols_third_party_detection_raw_wrapper_proto_depIdxs = []int32{
	1, // 0: protocols.third_party.detection.SSL_WrapperPacket.detection:type_name -> protocols.third_party.detection.SSL_DetectionFrame
	2, // 1: protocols.third_party.detection.SSL_WrapperPacket.geometry:type_name -> protocols.third_party.detection.SSL_GeometryData
	2, // [2:2] is the sub-list for method output_type
	2, // [2:2] is the sub-list for method input_type
	2, // [2:2] is the sub-list for extension type_name
	2, // [2:2] is the sub-list for extension extendee
	0, // [0:2] is the sub-list for field type_name
}

func init() { file_protocols_third_party_detection_raw_wrapper_proto_init() }
func file_protocols_third_party_detection_raw_wrapper_proto_init() {
	if File_protocols_third_party_detection_raw_wrapper_proto != nil {
		return
	}
	file_protocols_third_party_detection_geometry_proto_init()
	file_protocols_third_party_detection_raw_proto_init()
	if !protoimpl.UnsafeEnabled {
		file_protocols_third_party_detection_raw_wrapper_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*SSL_WrapperPacket); i {
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
			RawDescriptor: file_protocols_third_party_detection_raw_wrapper_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   1,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_protocols_third_party_detection_raw_wrapper_proto_goTypes,
		DependencyIndexes: file_protocols_third_party_detection_raw_wrapper_proto_depIdxs,
		MessageInfos:      file_protocols_third_party_detection_raw_wrapper_proto_msgTypes,
	}.Build()
	File_protocols_third_party_detection_raw_wrapper_proto = out.File
	file_protocols_third_party_detection_raw_wrapper_proto_rawDesc = nil
	file_protocols_third_party_detection_raw_wrapper_proto_goTypes = nil
	file_protocols_third_party_detection_raw_wrapper_proto_depIdxs = nil
}
