// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: my_python.proto

#include "my_python.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace Gamedata {
class PyTickInputInfoDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<PyTickInputInfo>
      _instance;
} _PyTickInputInfo_default_instance_;
class PyEntityInfoDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<PyEntityInfo>
      _instance;
} _PyEntityInfo_default_instance_;
}  // namespace Gamedata
namespace protobuf_my_5fpython_2eproto {
void InitDefaultsPyTickInputInfoImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  protobuf_the_5fpublic_2eproto::InitDefaultsFTransform();
  {
    void* ptr = &::Gamedata::_PyTickInputInfo_default_instance_;
    new (ptr) ::Gamedata::PyTickInputInfo();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::Gamedata::PyTickInputInfo::InitAsDefaultInstance();
}

void InitDefaultsPyTickInputInfo() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsPyTickInputInfoImpl);
}

void InitDefaultsPyEntityInfoImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  protobuf_the_5fpublic_2eproto::InitDefaultsFTransform();
  {
    void* ptr = &::Gamedata::_PyEntityInfo_default_instance_;
    new (ptr) ::Gamedata::PyEntityInfo();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::Gamedata::PyEntityInfo::InitAsDefaultInstance();
}

void InitDefaultsPyEntityInfo() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsPyEntityInfoImpl);
}

::google::protobuf::Metadata file_level_metadata[2];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyTickInputInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyTickInputInfo, entity_id_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyTickInputInfo, transform_),
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyEntityInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyEntityInfo, entity_id_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyEntityInfo, now_time_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyEntityInfo, use_world_transform_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyEntityInfo, w_transform_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Gamedata::PyEntityInfo, r_transform_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::Gamedata::PyTickInputInfo)},
  { 7, -1, sizeof(::Gamedata::PyEntityInfo)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::Gamedata::_PyTickInputInfo_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&::Gamedata::_PyEntityInfo_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "my_python.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 2);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\017my_python.proto\022\010Gamedata\032\020the_public."
      "proto\"M\n\017PyTickInputInfo\022\021\n\tentity_id\030\001 "
      "\001(\005\022\'\n\ttransform\030\004 \001(\0132\024.Gamedata.FTrans"
      "form\"\246\001\n\014PyEntityInfo\022\021\n\tentity_id\030\001 \001(\005"
      "\022\020\n\010now_time\030\002 \001(\002\022\033\n\023use_world_transfor"
      "m\030\003 \001(\010\022)\n\013w_transform\030\004 \001(\0132\024.Gamedata."
      "FTransform\022)\n\013r_transform\030\005 \001(\0132\024.Gameda"
      "ta.FTransformB\022\n\020zc.test.Protobufb\006proto"
      "3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 321);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "my_python.proto", &protobuf_RegisterTypes);
  ::protobuf_the_5fpublic_2eproto::AddDescriptors();
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_my_5fpython_2eproto
namespace Gamedata {

// ===================================================================

void PyTickInputInfo::InitAsDefaultInstance() {
  ::Gamedata::_PyTickInputInfo_default_instance_._instance.get_mutable()->transform_ = const_cast< ::Gamedata::FTransform*>(
      ::Gamedata::FTransform::internal_default_instance());
}
void PyTickInputInfo::clear_transform() {
  if (GetArenaNoVirtual() == NULL && transform_ != NULL) {
    delete transform_;
  }
  transform_ = NULL;
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int PyTickInputInfo::kEntityIdFieldNumber;
const int PyTickInputInfo::kTransformFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

PyTickInputInfo::PyTickInputInfo()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_my_5fpython_2eproto::InitDefaultsPyTickInputInfo();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:Gamedata.PyTickInputInfo)
}
PyTickInputInfo::PyTickInputInfo(const PyTickInputInfo& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.has_transform()) {
    transform_ = new ::Gamedata::FTransform(*from.transform_);
  } else {
    transform_ = NULL;
  }
  entity_id_ = from.entity_id_;
  // @@protoc_insertion_point(copy_constructor:Gamedata.PyTickInputInfo)
}

void PyTickInputInfo::SharedCtor() {
  ::memset(&transform_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&entity_id_) -
      reinterpret_cast<char*>(&transform_)) + sizeof(entity_id_));
  _cached_size_ = 0;
}

PyTickInputInfo::~PyTickInputInfo() {
  // @@protoc_insertion_point(destructor:Gamedata.PyTickInputInfo)
  SharedDtor();
}

void PyTickInputInfo::SharedDtor() {
  if (this != internal_default_instance()) delete transform_;
}

void PyTickInputInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PyTickInputInfo::descriptor() {
  ::protobuf_my_5fpython_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_my_5fpython_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const PyTickInputInfo& PyTickInputInfo::default_instance() {
  ::protobuf_my_5fpython_2eproto::InitDefaultsPyTickInputInfo();
  return *internal_default_instance();
}

PyTickInputInfo* PyTickInputInfo::New(::google::protobuf::Arena* arena) const {
  PyTickInputInfo* n = new PyTickInputInfo;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void PyTickInputInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:Gamedata.PyTickInputInfo)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaNoVirtual() == NULL && transform_ != NULL) {
    delete transform_;
  }
  transform_ = NULL;
  entity_id_ = 0;
  _internal_metadata_.Clear();
}

bool PyTickInputInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Gamedata.PyTickInputInfo)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 entity_id = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &entity_id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .Gamedata.FTransform transform = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 34 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_transform()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Gamedata.PyTickInputInfo)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Gamedata.PyTickInputInfo)
  return false;
#undef DO_
}

void PyTickInputInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Gamedata.PyTickInputInfo)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 entity_id = 1;
  if (this->entity_id() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->entity_id(), output);
  }

  // .Gamedata.FTransform transform = 4;
  if (this->has_transform()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      4, *this->transform_, output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:Gamedata.PyTickInputInfo)
}

::google::protobuf::uint8* PyTickInputInfo::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:Gamedata.PyTickInputInfo)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 entity_id = 1;
  if (this->entity_id() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->entity_id(), target);
  }

  // .Gamedata.FTransform transform = 4;
  if (this->has_transform()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        4, *this->transform_, deterministic, target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Gamedata.PyTickInputInfo)
  return target;
}

size_t PyTickInputInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Gamedata.PyTickInputInfo)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // .Gamedata.FTransform transform = 4;
  if (this->has_transform()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *this->transform_);
  }

  // int32 entity_id = 1;
  if (this->entity_id() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->entity_id());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PyTickInputInfo::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:Gamedata.PyTickInputInfo)
  GOOGLE_DCHECK_NE(&from, this);
  const PyTickInputInfo* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const PyTickInputInfo>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:Gamedata.PyTickInputInfo)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:Gamedata.PyTickInputInfo)
    MergeFrom(*source);
  }
}

void PyTickInputInfo::MergeFrom(const PyTickInputInfo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Gamedata.PyTickInputInfo)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_transform()) {
    mutable_transform()->::Gamedata::FTransform::MergeFrom(from.transform());
  }
  if (from.entity_id() != 0) {
    set_entity_id(from.entity_id());
  }
}

void PyTickInputInfo::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:Gamedata.PyTickInputInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PyTickInputInfo::CopyFrom(const PyTickInputInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Gamedata.PyTickInputInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PyTickInputInfo::IsInitialized() const {
  return true;
}

void PyTickInputInfo::Swap(PyTickInputInfo* other) {
  if (other == this) return;
  InternalSwap(other);
}
void PyTickInputInfo::InternalSwap(PyTickInputInfo* other) {
  using std::swap;
  swap(transform_, other->transform_);
  swap(entity_id_, other->entity_id_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata PyTickInputInfo::GetMetadata() const {
  protobuf_my_5fpython_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_my_5fpython_2eproto::file_level_metadata[kIndexInFileMessages];
}


// ===================================================================

void PyEntityInfo::InitAsDefaultInstance() {
  ::Gamedata::_PyEntityInfo_default_instance_._instance.get_mutable()->w_transform_ = const_cast< ::Gamedata::FTransform*>(
      ::Gamedata::FTransform::internal_default_instance());
  ::Gamedata::_PyEntityInfo_default_instance_._instance.get_mutable()->r_transform_ = const_cast< ::Gamedata::FTransform*>(
      ::Gamedata::FTransform::internal_default_instance());
}
void PyEntityInfo::clear_w_transform() {
  if (GetArenaNoVirtual() == NULL && w_transform_ != NULL) {
    delete w_transform_;
  }
  w_transform_ = NULL;
}
void PyEntityInfo::clear_r_transform() {
  if (GetArenaNoVirtual() == NULL && r_transform_ != NULL) {
    delete r_transform_;
  }
  r_transform_ = NULL;
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int PyEntityInfo::kEntityIdFieldNumber;
const int PyEntityInfo::kNowTimeFieldNumber;
const int PyEntityInfo::kUseWorldTransformFieldNumber;
const int PyEntityInfo::kWTransformFieldNumber;
const int PyEntityInfo::kRTransformFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

PyEntityInfo::PyEntityInfo()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_my_5fpython_2eproto::InitDefaultsPyEntityInfo();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:Gamedata.PyEntityInfo)
}
PyEntityInfo::PyEntityInfo(const PyEntityInfo& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.has_w_transform()) {
    w_transform_ = new ::Gamedata::FTransform(*from.w_transform_);
  } else {
    w_transform_ = NULL;
  }
  if (from.has_r_transform()) {
    r_transform_ = new ::Gamedata::FTransform(*from.r_transform_);
  } else {
    r_transform_ = NULL;
  }
  ::memcpy(&entity_id_, &from.entity_id_,
    static_cast<size_t>(reinterpret_cast<char*>(&use_world_transform_) -
    reinterpret_cast<char*>(&entity_id_)) + sizeof(use_world_transform_));
  // @@protoc_insertion_point(copy_constructor:Gamedata.PyEntityInfo)
}

void PyEntityInfo::SharedCtor() {
  ::memset(&w_transform_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&use_world_transform_) -
      reinterpret_cast<char*>(&w_transform_)) + sizeof(use_world_transform_));
  _cached_size_ = 0;
}

PyEntityInfo::~PyEntityInfo() {
  // @@protoc_insertion_point(destructor:Gamedata.PyEntityInfo)
  SharedDtor();
}

void PyEntityInfo::SharedDtor() {
  if (this != internal_default_instance()) delete w_transform_;
  if (this != internal_default_instance()) delete r_transform_;
}

void PyEntityInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PyEntityInfo::descriptor() {
  ::protobuf_my_5fpython_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_my_5fpython_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const PyEntityInfo& PyEntityInfo::default_instance() {
  ::protobuf_my_5fpython_2eproto::InitDefaultsPyEntityInfo();
  return *internal_default_instance();
}

PyEntityInfo* PyEntityInfo::New(::google::protobuf::Arena* arena) const {
  PyEntityInfo* n = new PyEntityInfo;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void PyEntityInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:Gamedata.PyEntityInfo)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaNoVirtual() == NULL && w_transform_ != NULL) {
    delete w_transform_;
  }
  w_transform_ = NULL;
  if (GetArenaNoVirtual() == NULL && r_transform_ != NULL) {
    delete r_transform_;
  }
  r_transform_ = NULL;
  ::memset(&entity_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&use_world_transform_) -
      reinterpret_cast<char*>(&entity_id_)) + sizeof(use_world_transform_));
  _internal_metadata_.Clear();
}

bool PyEntityInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Gamedata.PyEntityInfo)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 entity_id = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &entity_id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // float now_time = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(21u /* 21 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &now_time_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool use_world_transform = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &use_world_transform_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .Gamedata.FTransform w_transform = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 34 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_w_transform()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .Gamedata.FTransform r_transform = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(42u /* 42 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_r_transform()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Gamedata.PyEntityInfo)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Gamedata.PyEntityInfo)
  return false;
#undef DO_
}

void PyEntityInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Gamedata.PyEntityInfo)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 entity_id = 1;
  if (this->entity_id() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->entity_id(), output);
  }

  // float now_time = 2;
  if (this->now_time() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(2, this->now_time(), output);
  }

  // bool use_world_transform = 3;
  if (this->use_world_transform() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->use_world_transform(), output);
  }

  // .Gamedata.FTransform w_transform = 4;
  if (this->has_w_transform()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      4, *this->w_transform_, output);
  }

  // .Gamedata.FTransform r_transform = 5;
  if (this->has_r_transform()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      5, *this->r_transform_, output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:Gamedata.PyEntityInfo)
}

::google::protobuf::uint8* PyEntityInfo::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:Gamedata.PyEntityInfo)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 entity_id = 1;
  if (this->entity_id() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->entity_id(), target);
  }

  // float now_time = 2;
  if (this->now_time() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(2, this->now_time(), target);
  }

  // bool use_world_transform = 3;
  if (this->use_world_transform() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(3, this->use_world_transform(), target);
  }

  // .Gamedata.FTransform w_transform = 4;
  if (this->has_w_transform()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        4, *this->w_transform_, deterministic, target);
  }

  // .Gamedata.FTransform r_transform = 5;
  if (this->has_r_transform()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        5, *this->r_transform_, deterministic, target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Gamedata.PyEntityInfo)
  return target;
}

size_t PyEntityInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Gamedata.PyEntityInfo)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // .Gamedata.FTransform w_transform = 4;
  if (this->has_w_transform()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *this->w_transform_);
  }

  // .Gamedata.FTransform r_transform = 5;
  if (this->has_r_transform()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *this->r_transform_);
  }

  // int32 entity_id = 1;
  if (this->entity_id() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->entity_id());
  }

  // float now_time = 2;
  if (this->now_time() != 0) {
    total_size += 1 + 4;
  }

  // bool use_world_transform = 3;
  if (this->use_world_transform() != 0) {
    total_size += 1 + 1;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PyEntityInfo::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:Gamedata.PyEntityInfo)
  GOOGLE_DCHECK_NE(&from, this);
  const PyEntityInfo* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const PyEntityInfo>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:Gamedata.PyEntityInfo)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:Gamedata.PyEntityInfo)
    MergeFrom(*source);
  }
}

void PyEntityInfo::MergeFrom(const PyEntityInfo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Gamedata.PyEntityInfo)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_w_transform()) {
    mutable_w_transform()->::Gamedata::FTransform::MergeFrom(from.w_transform());
  }
  if (from.has_r_transform()) {
    mutable_r_transform()->::Gamedata::FTransform::MergeFrom(from.r_transform());
  }
  if (from.entity_id() != 0) {
    set_entity_id(from.entity_id());
  }
  if (from.now_time() != 0) {
    set_now_time(from.now_time());
  }
  if (from.use_world_transform() != 0) {
    set_use_world_transform(from.use_world_transform());
  }
}

void PyEntityInfo::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:Gamedata.PyEntityInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PyEntityInfo::CopyFrom(const PyEntityInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Gamedata.PyEntityInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PyEntityInfo::IsInitialized() const {
  return true;
}

void PyEntityInfo::Swap(PyEntityInfo* other) {
  if (other == this) return;
  InternalSwap(other);
}
void PyEntityInfo::InternalSwap(PyEntityInfo* other) {
  using std::swap;
  swap(w_transform_, other->w_transform_);
  swap(r_transform_, other->r_transform_);
  swap(entity_id_, other->entity_id_);
  swap(now_time_, other->now_time_);
  swap(use_world_transform_, other->use_world_transform_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata PyEntityInfo::GetMetadata() const {
  protobuf_my_5fpython_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_my_5fpython_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Gamedata

// @@protoc_insertion_point(global_scope)
