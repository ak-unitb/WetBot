<?php
    require_once('../incs/db.php');

	header("Content-Type: application/json");

    if ($conn) {
	
		$sensorId = 1;
		if (!empty($_GET['sensor_id'])) {
			$sensorId = $_GET['sensor_id'];
		}

		$sql = "SELECT * FROM `sensors`WHERE `id` = $sensorId";

		$rs = $conn->query($sql);

		if ($rs === false) {
			trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
		} else {
			$rs->data_seek(0);
			$loopCounter = 0;
			echo '{ "sensor": ';
			while ($row = $rs->fetch_assoc()) {

				echo '{ "id": ' . $row['id'] . ', "sensor_pin_number": ' . $row['sensor_pin_number'] . ', "relay_pin_number": ' . $row['relay_pin_number'] . ', "frequency_threeshold_too_dry": ' . $row['frequency_threeshold_too_dry'] . ', "frequency_threeshold_too_wet": ' . $row['frequency_threeshold_too_wet'] . ', "active": ' . ($row['active'] ? '1' : '0') . '}';
				$loopCounter++;
			}
		echo '}';

		}
	}
?>