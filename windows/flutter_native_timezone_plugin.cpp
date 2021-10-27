#include "include/flutter_native_timezone/flutter_native_timezone_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include <chrono>
#include <timezoneapi.h>
#include <stringapiset.h>


namespace {

class FlutterNativeTimezonePlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  FlutterNativeTimezonePlugin();

  virtual ~FlutterNativeTimezonePlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void FlutterNativeTimezonePlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "flutter_native_timezone",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<FlutterNativeTimezonePlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

FlutterNativeTimezonePlugin::FlutterNativeTimezonePlugin() {}

FlutterNativeTimezonePlugin::~FlutterNativeTimezonePlugin() {}

void FlutterNativeTimezonePlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getWindowsStandardTimezone") == 0) {

    DYNAMIC_TIME_ZONE_INFORMATION tzi;
    GetDynamicTimeZoneInformation(&tzi);

    auto timez = tzi.TimeZoneKeyName;
    char ch[260];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP,0,timez,-1, ch,260,&DefChar, NULL);

    std::string ans(ch);
    result->Success(flutter::EncodableValue(ans));
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void FlutterNativeTimezonePluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  FlutterNativeTimezonePlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
