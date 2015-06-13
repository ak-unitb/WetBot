/*
  SaveSensorData - Execute a script installed on th linino-/Yùn-side to insert data into db
  Created by count0, June 11, 2015.
  Released into the public domain.
*/

void initSaveSensorData();
bool insertSensorDataByPhpCli(long frequency, int gradeOfDryness, char* comment, int sensorNumber);
