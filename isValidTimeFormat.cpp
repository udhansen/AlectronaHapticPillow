#include "isValidTimeFormat.h"

// Function to validate HH:MM time format
bool isValidTimeFormat(String timeStr) {
  if (timeStr.length() != 5 || timeStr.charAt(2) != ':') return false;

  String hours = timeStr.substring(0, 2);
  String minutes = timeStr.substring(3, 5);

  int hour = hours.toInt();
  int minute = minutes.toInt();

  return (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59);
}