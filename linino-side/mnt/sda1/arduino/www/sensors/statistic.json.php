<?php
    require_once('../incs/db.php');
    require_once('sensor.class.php');

	header("Content-Type: application/json");

    if ($conn) {

		if (!empty($_GET['sensor_id'])) {
			$sensor = new Sensor();
			$sql = "SELECT * FROM `sensors` WHERE `id` = " . $_GET['sensor_id'];

			$rs = $conn->query($sql);

			if ($rs === false) {
				trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
			} else {
				$rs->data_seek(0);
				while ($row = $rs->fetch_assoc()) {
					$sensor->id = $row['id'];
					$sensor->name = $row['name'];
					$sensor->location_description = $row['location_description'];
					$sensor->sensor_pin_number = $row['sensor_pin_number'];
					$sensor->relay_pin_number = $row['relay_pin_number'];
					$sensor->frequency_threeshold_too_dry = $row['frequency_threeshold_too_dry'];
					$sensor->frequency_threeshold_too_wet = $row['frequency_threeshold_too_wet'];
					$sensor->active = $row['active'];
				}

				$sql = "SELECT `sd`.*, `s`.`name` as `sensor_name` FROM `sensor_datas` `sd`, `sensors` `s` WHERE `sd`.`sensor_id` = " . $_GET['sensor_id'] . " AND `sd`.`sensor_id` = `s`.`id` ORDER BY `sd`.`created_at` DESC LIMIT 720";
				$rs = $conn->query($sql);
		        if ($rs === false) {
		            trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
		        } else {
		            $rs->data_seek(0);
		
					$fromToDatetime = '';
					$toDateTime = '';
					$isFirstPassed = false;
		
					// sensorTitle, fromToDateTime, thresholdTooWet, thresholdTooDry, 
					echo
					'{
						"sensor_name": "' . $sensor->name . '",
						"thresholdTooDry": ' . $sensor->frequency_threeshold_too_dry . ',
						"thresholdTooWet": ' . $sensor->frequency_threeshold_too_wet . ',
						"data": [';
					while ($row = $rs->fetch_assoc()) {
						if (!$isFirstPassed) {
							$fromToDatetime = $row['created_at'];
						}
						echo $isFirstPassed ? ',' : '';
						echo
							'[' . strtotime($row['created_at']) . '000, ' . $row['frequency'] . "]\n"; // echo $row['created_at']
						$isFirstPassed = true;
						$toDateTime = $row['created_at'];
					}
					echo '],';
					echo
						'"fromToDatetime": [
							{ "timestamp": '. strtotime($toDateTime) . ',  "date": "' . $toDateTime . '"},
							{ "timestamp": ' . strtotime($fromToDatetime) . ', "date": "' . $fromToDatetime . '"}
						]
					}';
#							{ "timestamp": ' . strtotime($fromToDatetime) . ', "date": "' . strftime('%d.%m.%Y %H:%M:%S', $fromToDatetime) . '"},
#							{ "timestamp": '. strtotime($toDateTime) . ',  "date": "' . strftime('%d.%m.%Y %H:%M:%S', $toDateTime) . '"}

				}
			}
		}
	}
?>