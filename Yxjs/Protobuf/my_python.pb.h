// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: my_python.proto

#ifndef PROTOBUF_my_5fpython_2eproto__INCLUDED
#define PROTOBUF_my_5fpython_2eproto__INCLUDED

#pragma warning(disable:4456)
#pragma warning(disable:4146)
#pragma warning(disable:4647)
#pragma warning(disable:4668)
#pragma warning(disable:4125)
#pragma warning(disable:4800)
#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "the_public.pb.h"
// @@protoc_insertion_point(includes)

namespace protobuf_my_5fpython_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsPyTickInputInfoImpl();
void InitDefaultsPyTickInputInfo();
void InitDefaultsPyEntityInfoImpl();
void InitDefaultsPyEntityInfo();
inline void InitDefaults() {
  InitDefaultsPyTickInputInfo();
  InitDefaultsPyEntityInfo();
}
}  // namespace protobuf_my_5fpython_2eproto
namespace Gamedata {
class PyEntityInfo;
class PyEntityInfoDefaultTypeInternal;
extern PyEntityInfoDefaultTypeInternal _PyEntityInfo_default_instance_;
class PyTickInputInfo;
class PyTickInputInfoDefaultTypeInternal;
extern PyTickInputInfoDefaultTypeInternal _PyTickInputInfo_default_instance_;
}  // namespace Gamedata
namespace Gamedata {

// ===================================================================

class PyTickInputInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Gamedata.PyTickInputInfo) */ {
 public:
  PyTickInputInfo();
  virtual ~PyTickInputInfo();

  PyTickInputInfo(const PyTickInputInfo& from);

  inline PyTickInputInfo& operator=(const PyTickInputInfo& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PyTickInputInfo(PyTickInputInfo&& from) noexcept
    : PyTickInputInfo() {
    *this = ::std::move(from);
  }

  inline PyTickInputInfo& operator=(PyTickInputInfo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const PyTickInputInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PyTickInputInfo* internal_default_instance() {
    return reinterpret_cast<const PyTickInputInfo*>(
               &_PyTickInputInfo_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(PyTickInputInfo* other);
  friend void swap(PyTickInputInfo& a, PyTickInputInfo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PyTickInputInfo* New() const PROTOBUF_FINAL { return New(NULL); }

  PyTickInputInfo* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const PyTickInputInfo& from);
  void MergeFrom(const PyTickInputInfo& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(PyTickInputInfo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // .Gamedata.FTransform transform = 4;
  bool has_transform() const;
  void clear_transform();
  static const int kTransformFieldNumber = 4;
  const ::Gamedata::FTransform& transform() const;
  ::Gamedata::FTransform* release_transform();
  ::Gamedata::FTransform* mutable_transform();
  void set_allocated_transform(::Gamedata::FTransform* transform);

  // int32 entity_id = 1;
  void clear_entity_id();
  static const int kEntityIdFieldNumber = 1;
  ::google::protobuf::int32 entity_id() const;
  void set_entity_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Gamedata.PyTickInputInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::Gamedata::FTransform* transform_;
  ::google::protobuf::int32 entity_id_;
  mutable int _cached_size_;
  friend struct ::protobuf_my_5fpython_2eproto::TableStruct;
  friend void ::protobuf_my_5fpython_2eproto::InitDefaultsPyTickInputInfoImpl();
};
// -------------------------------------------------------------------

class PyEntityInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Gamedata.PyEntityInfo) */ {
 public:
  PyEntityInfo();
  virtual ~PyEntityInfo();

  PyEntityInfo(const PyEntityInfo& from);

  inline PyEntityInfo& operator=(const PyEntityInfo& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PyEntityInfo(PyEntityInfo&& from) noexcept
    : PyEntityInfo() {
    *this = ::std::move(from);
  }

  inline PyEntityInfo& operator=(PyEntityInfo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const PyEntityInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PyEntityInfo* internal_default_instance() {
    return reinterpret_cast<const PyEntityInfo*>(
               &_PyEntityInfo_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(PyEntityInfo* other);
  friend void swap(PyEntityInfo& a, PyEntityInfo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PyEntityInfo* New() const PROTOBUF_FINAL { return New(NULL); }

  PyEntityInfo* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const PyEntityInfo& from);
  void MergeFrom(const PyEntityInfo& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(PyEntityInfo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // .Gamedata.FTransform w_transform = 4;
  bool has_w_transform() const;
  void clear_w_transform();
  static const int kWTransformFieldNumber = 4;
  const ::Gamedata::FTransform& w_transform() const;
  ::Gamedata::FTransform* release_w_transform();
  ::Gamedata::FTransform* mutable_w_transform();
  void set_allocated_w_transform(::Gamedata::FTransform* w_transform);

  // .Gamedata.FTransform r_transform = 5;
  bool has_r_transform() const;
  void clear_r_transform();
  static const int kRTransformFieldNumber = 5;
  const ::Gamedata::FTransform& r_transform() const;
  ::Gamedata::FTransform* release_r_transform();
  ::Gamedata::FTransform* mutable_r_transform();
  void set_allocated_r_transform(::Gamedata::FTransform* r_transform);

  // int32 entity_id = 1;
  void clear_entity_id();
  static const int kEntityIdFieldNumber = 1;
  ::google::protobuf::int32 entity_id() const;
  void set_entity_id(::google::protobuf::int32 value);

  // float now_time = 2;
  void clear_now_time();
  static const int kNowTimeFieldNumber = 2;
  float now_time() const;
  void set_now_time(float value);

  // bool use_world_transform = 3;
  void clear_use_world_transform();
  static const int kUseWorldTransformFieldNumber = 3;
  bool use_world_transform() const;
  void set_use_world_transform(bool value);

  // @@protoc_insertion_point(class_scope:Gamedata.PyEntityInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::Gamedata::FTransform* w_transform_;
  ::Gamedata::FTransform* r_transform_;
  ::google::protobuf::int32 entity_id_;
  float now_time_;
  bool use_world_transform_;
  mutable int _cached_size_;
  friend struct ::protobuf_my_5fpython_2eproto::TableStruct;
  friend void ::protobuf_my_5fpython_2eproto::InitDefaultsPyEntityInfoImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// PyTickInputInfo

// int32 entity_id = 1;
inline void PyTickInputInfo::clear_entity_id() {
  entity_id_ = 0;
}
inline ::google::protobuf::int32 PyTickInputInfo::entity_id() const {
  // @@protoc_insertion_point(field_get:Gamedata.PyTickInputInfo.entity_id)
  return entity_id_;
}
inline void PyTickInputInfo::set_entity_id(::google::protobuf::int32 value) {
  
  entity_id_ = value;
  // @@protoc_insertion_point(field_set:Gamedata.PyTickInputInfo.entity_id)
}

// .Gamedata.FTransform transform = 4;
inline bool PyTickInputInfo::has_transform() const {
  return this != internal_default_instance() && transform_ != NULL;
}
inline const ::Gamedata::FTransform& PyTickInputInfo::transform() const {
  const ::Gamedata::FTransform* p = transform_;
  // @@protoc_insertion_point(field_get:Gamedata.PyTickInputInfo.transform)
  return p != NULL ? *p : *reinterpret_cast<const ::Gamedata::FTransform*>(
      &::Gamedata::_FTransform_default_instance_);
}
inline ::Gamedata::FTransform* PyTickInputInfo::release_transform() {
  // @@protoc_insertion_point(field_release:Gamedata.PyTickInputInfo.transform)
  
  ::Gamedata::FTransform* temp = transform_;
  transform_ = NULL;
  return temp;
}
inline ::Gamedata::FTransform* PyTickInputInfo::mutable_transform() {
  
  if (transform_ == NULL) {
    transform_ = new ::Gamedata::FTransform;
  }
  // @@protoc_insertion_point(field_mutable:Gamedata.PyTickInputInfo.transform)
  return transform_;
}
inline void PyTickInputInfo::set_allocated_transform(::Gamedata::FTransform* transform) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(transform_);
  }
  if (transform) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      transform = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, transform, submessage_arena);
    }
    
  } else {
    
  }
  transform_ = transform;
  // @@protoc_insertion_point(field_set_allocated:Gamedata.PyTickInputInfo.transform)
}

// -------------------------------------------------------------------

// PyEntityInfo

// int32 entity_id = 1;
inline void PyEntityInfo::clear_entity_id() {
  entity_id_ = 0;
}
inline ::google::protobuf::int32 PyEntityInfo::entity_id() const {
  // @@protoc_insertion_point(field_get:Gamedata.PyEntityInfo.entity_id)
  return entity_id_;
}
inline void PyEntityInfo::set_entity_id(::google::protobuf::int32 value) {
  
  entity_id_ = value;
  // @@protoc_insertion_point(field_set:Gamedata.PyEntityInfo.entity_id)
}

// float now_time = 2;
inline void PyEntityInfo::clear_now_time() {
  now_time_ = 0;
}
inline float PyEntityInfo::now_time() const {
  // @@protoc_insertion_point(field_get:Gamedata.PyEntityInfo.now_time)
  return now_time_;
}
inline void PyEntityInfo::set_now_time(float value) {
  
  now_time_ = value;
  // @@protoc_insertion_point(field_set:Gamedata.PyEntityInfo.now_time)
}

// bool use_world_transform = 3;
inline void PyEntityInfo::clear_use_world_transform() {
  use_world_transform_ = false;
}
inline bool PyEntityInfo::use_world_transform() const {
  // @@protoc_insertion_point(field_get:Gamedata.PyEntityInfo.use_world_transform)
  return use_world_transform_;
}
inline void PyEntityInfo::set_use_world_transform(bool value) {
  
  use_world_transform_ = value;
  // @@protoc_insertion_point(field_set:Gamedata.PyEntityInfo.use_world_transform)
}

// .Gamedata.FTransform w_transform = 4;
inline bool PyEntityInfo::has_w_transform() const {
  return this != internal_default_instance() && w_transform_ != NULL;
}
inline const ::Gamedata::FTransform& PyEntityInfo::w_transform() const {
  const ::Gamedata::FTransform* p = w_transform_;
  // @@protoc_insertion_point(field_get:Gamedata.PyEntityInfo.w_transform)
  return p != NULL ? *p : *reinterpret_cast<const ::Gamedata::FTransform*>(
      &::Gamedata::_FTransform_default_instance_);
}
inline ::Gamedata::FTransform* PyEntityInfo::release_w_transform() {
  // @@protoc_insertion_point(field_release:Gamedata.PyEntityInfo.w_transform)
  
  ::Gamedata::FTransform* temp = w_transform_;
  w_transform_ = NULL;
  return temp;
}
inline ::Gamedata::FTransform* PyEntityInfo::mutable_w_transform() {
  
  if (w_transform_ == NULL) {
    w_transform_ = new ::Gamedata::FTransform;
  }
  // @@protoc_insertion_point(field_mutable:Gamedata.PyEntityInfo.w_transform)
  return w_transform_;
}
inline void PyEntityInfo::set_allocated_w_transform(::Gamedata::FTransform* w_transform) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(w_transform_);
  }
  if (w_transform) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      w_transform = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, w_transform, submessage_arena);
    }
    
  } else {
    
  }
  w_transform_ = w_transform;
  // @@protoc_insertion_point(field_set_allocated:Gamedata.PyEntityInfo.w_transform)
}

// .Gamedata.FTransform r_transform = 5;
inline bool PyEntityInfo::has_r_transform() const {
  return this != internal_default_instance() && r_transform_ != NULL;
}
inline const ::Gamedata::FTransform& PyEntityInfo::r_transform() const {
  const ::Gamedata::FTransform* p = r_transform_;
  // @@protoc_insertion_point(field_get:Gamedata.PyEntityInfo.r_transform)
  return p != NULL ? *p : *reinterpret_cast<const ::Gamedata::FTransform*>(
      &::Gamedata::_FTransform_default_instance_);
}
inline ::Gamedata::FTransform* PyEntityInfo::release_r_transform() {
  // @@protoc_insertion_point(field_release:Gamedata.PyEntityInfo.r_transform)
  
  ::Gamedata::FTransform* temp = r_transform_;
  r_transform_ = NULL;
  return temp;
}
inline ::Gamedata::FTransform* PyEntityInfo::mutable_r_transform() {
  
  if (r_transform_ == NULL) {
    r_transform_ = new ::Gamedata::FTransform;
  }
  // @@protoc_insertion_point(field_mutable:Gamedata.PyEntityInfo.r_transform)
  return r_transform_;
}
inline void PyEntityInfo::set_allocated_r_transform(::Gamedata::FTransform* r_transform) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(r_transform_);
  }
  if (r_transform) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      r_transform = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, r_transform, submessage_arena);
    }
    
  } else {
    
  }
  r_transform_ = r_transform;
  // @@protoc_insertion_point(field_set_allocated:Gamedata.PyEntityInfo.r_transform)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Gamedata

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_my_5fpython_2eproto__INCLUDED
