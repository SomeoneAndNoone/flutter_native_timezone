import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:flutter/services.dart';
import 'package:flutter_native_timezone/constants.dart';

///
/// Class for getting the native timezone.
///
class FlutterNativeTimezone {
  static const MethodChannel _channel = const MethodChannel('flutter_native_timezone');

  static List<dynamic>? mappedTimezones;

  /// Returns local timezone from the native layer.
  ///
  static Future<String> getLocalTimezone() async {
    if (Platform.isWindows) {
      final String? windowsTimezone = await _channel.invokeMethod("getWindowsStandardTimezone");
      if (windowsTimezone == null) {
        throw ArgumentError(
            "Invalid return from platform getWindowsStandardTimezone. Native Windows");
      }

      if (mappedTimezones == null) {
        mappedTimezones = await json.decode(DATA_STR);
      }

      try {
        return mappedTimezones!.firstWhere((map) => map['windowsName'] == windowsTimezone)['iana']
            [0] as String;
      } catch (e) {
        print('Unexpected error happened in parsing timezones. flutter_native_timezone.dart');
        return 'Unknown timezone from windows';
      }
    }
    final String? localTimezone = await _channel.invokeMethod("getLocalTimezone");
    if (localTimezone == null) {
      throw ArgumentError("Invalid return from platform getLocalTimezone()");
    }
    return localTimezone;
  }

  ///
  /// Gets the list of available timezones from the native layer.
  ///
  static Future<List<String>> getAvailableTimezones() async {
    if (Platform.isWindows) {
      print('Unimplemented in windows');
      return [];
    }
    final List<String>? availableTimezones =
        await _channel.invokeListMethod<String>("getAvailableTimezones");
    if (availableTimezones == null) {
      throw ArgumentError("Invalid return from platform getAvailableTimezones()");
    }
    return availableTimezones;
  }
}
