// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.33.0
// 	protoc        v4.25.3
// source: match.proto

package proto

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

type Match struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Id   string `protobuf:"bytes,1,opt,name=id,proto3" json:"id,omitempty"`
	Name string `protobuf:"bytes,2,opt,name=name,proto3" json:"name,omitempty"`
}

func (x *Match) Reset() {
	*x = Match{}
	if protoimpl.UnsafeEnabled {
		mi := &file_match_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Match) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Match) ProtoMessage() {}

func (x *Match) ProtoReflect() protoreflect.Message {
	mi := &file_match_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Match.ProtoReflect.Descriptor instead.
func (*Match) Descriptor() ([]byte, []int) {
	return file_match_proto_rawDescGZIP(), []int{0}
}

func (x *Match) GetId() string {
	if x != nil {
		return x.Id
	}
	return ""
}

func (x *Match) GetName() string {
	if x != nil {
		return x.Name
	}
	return ""
}

type MatchRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Req string `protobuf:"bytes,1,opt,name=req,proto3" json:"req,omitempty"`
}

func (x *MatchRequest) Reset() {
	*x = MatchRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_match_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *MatchRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*MatchRequest) ProtoMessage() {}

func (x *MatchRequest) ProtoReflect() protoreflect.Message {
	mi := &file_match_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use MatchRequest.ProtoReflect.Descriptor instead.
func (*MatchRequest) Descriptor() ([]byte, []int) {
	return file_match_proto_rawDescGZIP(), []int{1}
}

func (x *MatchRequest) GetReq() string {
	if x != nil {
		return x.Req
	}
	return ""
}

type MatchResponse struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Match *Match `protobuf:"bytes,1,opt,name=match,proto3" json:"match,omitempty"`
}

func (x *MatchResponse) Reset() {
	*x = MatchResponse{}
	if protoimpl.UnsafeEnabled {
		mi := &file_match_proto_msgTypes[2]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *MatchResponse) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*MatchResponse) ProtoMessage() {}

func (x *MatchResponse) ProtoReflect() protoreflect.Message {
	mi := &file_match_proto_msgTypes[2]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use MatchResponse.ProtoReflect.Descriptor instead.
func (*MatchResponse) Descriptor() ([]byte, []int) {
	return file_match_proto_rawDescGZIP(), []int{2}
}

func (x *MatchResponse) GetMatch() *Match {
	if x != nil {
		return x.Match
	}
	return nil
}

var File_match_proto protoreflect.FileDescriptor

var file_match_proto_rawDesc = []byte{
	0x0a, 0x0b, 0x6d, 0x61, 0x74, 0x63, 0x68, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x02, 0x76,
	0x31, 0x22, 0x2b, 0x0a, 0x05, 0x4d, 0x61, 0x74, 0x63, 0x68, 0x12, 0x0e, 0x0a, 0x02, 0x69, 0x64,
	0x18, 0x01, 0x20, 0x01, 0x28, 0x09, 0x52, 0x02, 0x69, 0x64, 0x12, 0x12, 0x0a, 0x04, 0x6e, 0x61,
	0x6d, 0x65, 0x18, 0x02, 0x20, 0x01, 0x28, 0x09, 0x52, 0x04, 0x6e, 0x61, 0x6d, 0x65, 0x22, 0x20,
	0x0a, 0x0c, 0x4d, 0x61, 0x74, 0x63, 0x68, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x12, 0x10,
	0x0a, 0x03, 0x72, 0x65, 0x71, 0x18, 0x01, 0x20, 0x01, 0x28, 0x09, 0x52, 0x03, 0x72, 0x65, 0x71,
	0x22, 0x30, 0x0a, 0x0d, 0x4d, 0x61, 0x74, 0x63, 0x68, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73,
	0x65, 0x12, 0x1f, 0x0a, 0x05, 0x6d, 0x61, 0x74, 0x63, 0x68, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b,
	0x32, 0x09, 0x2e, 0x76, 0x31, 0x2e, 0x4d, 0x61, 0x74, 0x63, 0x68, 0x52, 0x05, 0x6d, 0x61, 0x74,
	0x63, 0x68, 0x32, 0x3c, 0x0a, 0x0c, 0x4d, 0x61, 0x74, 0x63, 0x68, 0x53, 0x65, 0x72, 0x76, 0x69,
	0x63, 0x65, 0x12, 0x2c, 0x0a, 0x03, 0x47, 0x65, 0x74, 0x12, 0x10, 0x2e, 0x76, 0x31, 0x2e, 0x4d,
	0x61, 0x74, 0x63, 0x68, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x1a, 0x11, 0x2e, 0x76, 0x31,
	0x2e, 0x4d, 0x61, 0x74, 0x63, 0x68, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x22, 0x00,
	0x42, 0x0a, 0x5a, 0x08, 0x76, 0x31, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62, 0x06, 0x70, 0x72,
	0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_match_proto_rawDescOnce sync.Once
	file_match_proto_rawDescData = file_match_proto_rawDesc
)

func file_match_proto_rawDescGZIP() []byte {
	file_match_proto_rawDescOnce.Do(func() {
		file_match_proto_rawDescData = protoimpl.X.CompressGZIP(file_match_proto_rawDescData)
	})
	return file_match_proto_rawDescData
}

var file_match_proto_msgTypes = make([]protoimpl.MessageInfo, 3)
var file_match_proto_goTypes = []interface{}{
	(*Match)(nil),         // 0: v1.Match
	(*MatchRequest)(nil),  // 1: v1.MatchRequest
	(*MatchResponse)(nil), // 2: v1.MatchResponse
}
var file_match_proto_depIdxs = []int32{
	0, // 0: v1.MatchResponse.match:type_name -> v1.Match
	1, // 1: v1.MatchService.Get:input_type -> v1.MatchRequest
	2, // 2: v1.MatchService.Get:output_type -> v1.MatchResponse
	2, // [2:3] is the sub-list for method output_type
	1, // [1:2] is the sub-list for method input_type
	1, // [1:1] is the sub-list for extension type_name
	1, // [1:1] is the sub-list for extension extendee
	0, // [0:1] is the sub-list for field type_name
}

func init() { file_match_proto_init() }
func file_match_proto_init() {
	if File_match_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_match_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Match); i {
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
		file_match_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*MatchRequest); i {
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
		file_match_proto_msgTypes[2].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*MatchResponse); i {
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
			RawDescriptor: file_match_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   3,
			NumExtensions: 0,
			NumServices:   1,
		},
		GoTypes:           file_match_proto_goTypes,
		DependencyIndexes: file_match_proto_depIdxs,
		MessageInfos:      file_match_proto_msgTypes,
	}.Build()
	File_match_proto = out.File
	file_match_proto_rawDesc = nil
	file_match_proto_goTypes = nil
	file_match_proto_depIdxs = nil
}
