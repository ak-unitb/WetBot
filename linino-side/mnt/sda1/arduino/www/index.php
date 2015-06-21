<?php
	require_once('incs/db.php');
?>
<?php include "incs/html_head.php" ?>

		<h1>Aktuelle Werte</h1>
<?php
	if ($conn) {
?>
<?php
		$sqlSensors = 'SELECT *  FROM `sensors` WHERE `active` = TRUE;';
		$rsSensors = $conn->query($sqlSensors);

		if ($rsSensors === false) {
			trigger_error('Wrong SQL: ' . $sqlSensors . ' Error: ' . $conn->error, E_USER_ERROR);
		} else {

			$rsSensors->data_seek(0);
			while ($rowSensors = $rsSensors->fetch_assoc()) {
				$sqlSensorDatas = "SELECT * FROM `sensor_datas` WHERE `sensor_id` = ".$rowSensors['id']." ORDER BY `created_at` DESC LIMIT 1";
				$rsSensorDatas = $conn->query($sqlSensorDatas);
				$sqlSensorTrend = "SELECT AVG(`last30`.`frequency`) AS `average_last30`,  AVG(`last5`.`frequency`) AS `average_last5`, (AVG(`last30`.`frequency`) - AVG(`last5`.`frequency`)) AS `delta`,
									(AVG(`last30`.`frequency`) - AVG(`last5`.`frequency`)) < 0 AS `getting_dryer`,
									(AVG(`last30`.`frequency`) - AVG(`last5`.`frequency`)) > 0 AS `getting_wetter`
									FROM (SELECT `frequency` FROM `sensor_datas` WHERE `sensor_id` = ".$rowSensors['id']." ORDER BY `created_at` DESC LIMIT 30) `last30`,
										(SELECT `frequency` FROM `sensor_datas` WHERE `sensor_id` = ".$rowSensors['id']." ORDER BY `created_at` DESC LIMIT 5) `last5`;";
				$rsSensorTrend = $conn->query($sqlSensorTrend);
				if ($rsSensorDatas === false || $rsSensorTrend=== false) {
					trigger_error('Wrong SQL: ' . $sqlSensorDatas . ' Error: ' . $conn->error, E_USER_ERROR);
				} else {
					$rsSensorDatas->data_seek(0);
					$rsSensorTrend->data_seek(0);
					$rowSensorDatas = $rsSensorDatas->fetch_assoc();
					$rowSensorTrend = $rsSensorTrend->fetch_assoc();
?>
		<div class="sensor-data" data-id="<?php echo $rowSensors['id'] ?>" data-pin-number="<?php echo $rowSensors['pin_number'] ?>">
			<ul class="list-inline pull-right">
				<li><a href="#" class="sensor-data__watering-button xhr-call" data-url="/arduino/digital/<?php echo $rowSensors['relay_pin_number'] ?>" title=""><span class="glyphicon glyphicon-tint"></span></a></li>
			</ul>
			<h2 class="sensor-data__name"><?php echo $rowSensors['name'] ?></h2>
			<p class="sensor-data__location-description">
				<?php echo $rowSensors['location_description'] ?>
			</p>
			<div
				class="sensor-data__status sensor-data__status--grade-of-dryness-<?php echo $rowSensorDatas['grade_of_dryness'] ?> clearfix"
				data-grade-of-dryness="Trockenheitsgrad: <?php echo $rowSensorDatas['grade_of_dryness'] ?>"
				data-title="Created @ <?php echo $rowSensorDatas['created_at'] ?>">
<?php
				   if($rowSensorTrend['getting_dryer']) {
?>
						<span class="sensor-data__status__trend sensor-data__status__trend--gettingdryer" title="Trend: wird trockener (<?php echo $rowSensorTrend['delta'] ?>)">
							<span class="glyphicon glyphicon-arrow-down"></span>
						</span>
<?php
				   } else if ($rowSensorTrend['getting_wetter']) {
?>
						<span class="sensor-data__status__trend sensor-data__status__trend--gettingwetter" title="Trend: wird feuchter (<?php echo $rowSensorTrend['delta'] ?>)">
							<span class="glyphicon glyphicon-arrow-up"></span>
						</span>
<?php
				   } else {
?>
						<span class="sensor-data__status__trend sensor-data__status__trend--constant" title="Trend: gleichbleibend (<?php echo $rowSensorTrend['delta'] ?>)">
							<span class="glyphicon glyphicon-arrow-right"></span>
						</span>
<?php
				   }
?>
				<span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--0">klatschnass</span>
				<span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--1">nass</span>
				<span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--2">feucht</span>
				<span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--3">trocken</span>
				<span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--4">staubig</span>
				<span class="sensor-data__status__frequency">Frequenz: <?php echo $rowSensorDatas['frequency'] ?> Hz</span>
				<span class="sensor-data__status__comment"><?php if (!empty($rowSensorDatas['comment'])) { ?><?php echo $rowSensorDatas['comment']; } else { ?><?php echo "&nbsp;"; } ?></span>
			</div>
			<ul class="list-inline">
				<li><a href="last-100.php?sensor_id=<?php echo $rowSensors['id'] ?>">letzte Werte</a></li>
				<li><a href="statistic-graphs.php?sensor_id=<?php echo $rowSensors['id'] ?>">Diagram</a></li>
			</ul>
		</div>
<?php
				}
			}
		}

	} else {
?>
		<p class="text-message has-error">Datenbank Verbindung fehlgeschlagen</p>
<?php
	}
?>
<?php include "incs/html_foot.php" ?>
