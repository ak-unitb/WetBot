<?php
    require_once('../incs/db.php');
    require_once('sensor.class.php');

	header("Content-Type: application/json");

    if ($conn) {

		$sql = "SELECT * FROM `sensors` ORDER BY `id`";

		$rs = $conn->query($sql);

		if ($rs === false) {
			trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
		} else {
			$rs->data_seek(0);
			$loopCounter = 0;
			echo '
{
	"sensors": [';
			while ($row = $rs->fetch_assoc()) {

				echo '
		' . ($loopCounter > 0 ? ',' : '') . '{
			"id": ' . $row['id'] . ',
			"name": "' . $row['name'] . '",
			"location_description": "' . $row['location_description'] . '",
			"sensor_pin_number": ' . $row['sensor_pin_number'] . ',
			"relay_pin_number": ' . $row['relay_pin_number'] . ',
			"frequency_threeshold_too_dry": ' . $row['frequency_threeshold_too_dry'] . ',
			"frequency_threeshold_too_wet": ' . $row['frequency_threeshold_too_wet'] . ',
			"active": ' . ($row['active'] ? 'true' : 'false') . '
		}';
			$loopCounter++;
			}
		echo '
	]
}';

		}
	}
?>