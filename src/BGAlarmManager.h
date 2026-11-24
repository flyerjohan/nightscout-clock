#ifndef BGAlarmManager_h
#define BGAlarmManager_h

#include <Arduino.h>
#include <SettingsAlarm.h>

#include <vector>

struct AlarmData {
    int bottom;
    int top;
    unsigned long lastAlarmTime;
    int snoozeTimeMinutes;
    std::vector<AlertWindow> alertWindows;
    String alarmSound;
    bool isSnoozed;
};

class BGAlarmManager_ {
private:
    BGAlarmManager_() = default;
    std::vector<AlarmData> enabledAlarms;
    AlarmData* activeAlarm;
    int alarmIntervalSeconds;
    // Sounds when no reading has come for a while, which the glucose alarms cannot notice.
    bool noDataAlarmEnabled = false;
    int noDataSeconds = 0;
    AlarmData noDataAlarm;
    bool wasSounding = false;

    void tickAlarms();
    void tickNoDataAlarm();

public:
    static BGAlarmManager_& getInstance();
    void setup();
    void tick();
    // An alarm is sounding and not snoozed.
    bool isSounding() const { return activeAlarm != NULL && !activeAlarm->isSnoozed; }
    // Returns whether an alarm was snoozed by this call.
    bool snoozeAlarm();
};

extern BGAlarmManager_& bgAlarmManager;
#endif
