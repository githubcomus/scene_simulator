// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test.proto

#ifndef PROTOBUF_test_2eproto__INCLUDED
#define PROTOBUF_test_2eproto__INCLUDED

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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_test_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsFinalMsgImpl();
void InitDefaultsFinalMsg();
inline void InitDefaults() {
  InitDefaultsFinalMsg();
}
}  // namespace protobuf_test_2eproto
namespace Gamedata {
class FinalMsg;
class FinalMsgDefaultTypeInternal;
extern FinalMsgDefaultTypeInternal _FinalMsg_default_instance_;
}  // namespace Gamedata
namespace Gamedata {

enum TestMsgType {
  Vec = 0,
  Pnt = 1,
  TestMsgType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  TestMsgType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool TestMsgType_IsValid(int value);
const TestMsgType TestMsgType_MIN = Vec;
const TestMsgType TestMsgType_MAX = Pnt;
const int TestMsgType_ARRAYSIZE = TestMsgType_MAX + 1;

const ::google::protobuf::EnumDescriptor* TestMsgType_descriptor();
inline const ::std::string& TestMsgType_Name(TestMsgType value) {
  return ::google::protobuf::internal::NameOfEnum(
    TestMsgType_descriptor(), value);
}
inline bool TestMsgType_Parse(
    const ::std::string& name, TestMsgType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<TestMsgType>(
    TestMsgType_descriptor(), name, value);
}
// ===================================================================

class FinalMsg : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Gamedata.FinalMsg) */ {
 public:
  FinalMsg();
  virtual ~FinalMsg();

  FinalMsg(const FinalMsg& from);

  inline FinalMsg& operator=(const FinalMsg& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  FinalMsg(FinalMsg&& from) noexcept
    : FinalMsg() {
    *this = ::std::move(from);
  }

  inline FinalMsg& operator=(FinalMsg&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const FinalMsg& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const FinalMsg* internal_default_instance() {
    return reinterpret_cast<const FinalMsg*>(
               &_FinalMsg_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(FinalMsg* other);
  friend void swap(FinalMsg& a, FinalMsg& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline FinalMsg* New() const PROTOBUF_FINAL { return New(NULL); }

  FinalMsg* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const FinalMsg& from);
  void MergeFrom(const FinalMsg& from);
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
  void InternalSwap(FinalMsg* other);
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

  // string username = 1;
  void clear_username();
  static const int kUsernameFieldNumber = 1;
  const ::std::string& username() const;
  void set_username(const ::std::string& value);
  #if LANG_CXX11
  void set_username(::std::string&& value);
  #endif
  void set_username(const char* value);
  void set_username(const char* value, size_t size);
  ::std::string* mutable_username();
  ::std::string* release_username();
  void set_allocated_username(::std::string* username);

  // string password = 2;
  void clear_password();
  static const int kPasswordFieldNumber = 2;
  const ::std::string& password() const;
  void set_password(const ::std::string& value);
  #if LANG_CXX11
  void set_password(::std::string&& value);
  #endif
  void set_password(const char* value);
  void set_password(const char* value, size_t size);
  ::std::string* mutable_password();
  ::std::string* release_password();
  void set_allocated_password(::std::string* password);

  // @@protoc_insertion_point(class_scope:Gamedata.FinalMsg)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr username_;
  ::google::protobuf::internal::ArenaStringPtr password_;
  mutable int _cached_size_;
  friend struct ::protobuf_test_2eproto::TableStruct;
  friend void ::protobuf_test_2eproto::InitDefaultsFinalMsgImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// FinalMsg

// string username = 1;
inline void FinalMsg::clear_username() {
  username_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& FinalMsg::username() const {
  // @@protoc_insertion_point(field_get:Gamedata.FinalMsg.username)
  return username_.GetNoArena();
}
inline void FinalMsg::set_username(const ::std::string& value) {
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Gamedata.FinalMsg.username)
}
#if LANG_CXX11
inline void FinalMsg::set_username(::std::string&& value) {
  
  username_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Gamedata.FinalMsg.username)
}
#endif
inline void FinalMsg::set_username(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Gamedata.FinalMsg.username)
}
inline void FinalMsg::set_username(const char* value, size_t size) {
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Gamedata.FinalMsg.username)
}
inline ::std::string* FinalMsg::mutable_username() {
  
  // @@protoc_insertion_point(field_mutable:Gamedata.FinalMsg.username)
  return username_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* FinalMsg::release_username() {
  // @@protoc_insertion_point(field_release:Gamedata.FinalMsg.username)
  
  return username_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void FinalMsg::set_allocated_username(::std::string* username) {
  if (username != NULL) {
    
  } else {
    
  }
  username_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), username);
  // @@protoc_insertion_point(field_set_allocated:Gamedata.FinalMsg.username)
}

// string password = 2;
inline void FinalMsg::clear_password() {
  password_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& FinalMsg::password() const {
  // @@protoc_insertion_point(field_get:Gamedata.FinalMsg.password)
  return password_.GetNoArena();
}
inline void FinalMsg::set_password(const ::std::string& value) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Gamedata.FinalMsg.password)
}
#if LANG_CXX11
inline void FinalMsg::set_password(::std::string&& value) {
  
  password_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Gamedata.FinalMsg.password)
}
#endif
inline void FinalMsg::set_password(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Gamedata.FinalMsg.password)
}
inline void FinalMsg::set_password(const char* value, size_t size) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Gamedata.FinalMsg.password)
}
inline ::std::string* FinalMsg::mutable_password() {
  
  // @@protoc_insertion_point(field_mutable:Gamedata.FinalMsg.password)
  return password_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* FinalMsg::release_password() {
  // @@protoc_insertion_point(field_release:Gamedata.FinalMsg.password)
  
  return password_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void FinalMsg::set_allocated_password(::std::string* password) {
  if (password != NULL) {
    
  } else {
    
  }
  password_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), password);
  // @@protoc_insertion_point(field_set_allocated:Gamedata.FinalMsg.password)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Gamedata

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::Gamedata::TestMsgType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Gamedata::TestMsgType>() {
  return ::Gamedata::TestMsgType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_test_2eproto__INCLUDED