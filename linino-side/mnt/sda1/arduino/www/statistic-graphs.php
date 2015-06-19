<?php
	require_once('incs/db.php');
?>
<?php include "incs/html_head.php" ?>

		<h1>Statistik</h1>
<?php
	if ($conn) {

		$sqlSensors = 'SELECT *  FROM `sensors` WHERE `active` = TRUE;';
		$rsSensors = $conn->query($sqlSensors);

		if ($rsSensors === false) {
			trigger_error('Wrong SQL: ' . $sqlSensors . ' Error: ' . $conn->error, E_USER_ERROR);
		} else {
?>
	<form action="statistic-graphs.php" method="get">
		<div class="form-group">
			<label for="name">Sensor</label>
			<select class="form-control" name="sensor_id" id="statisticsGraphSensorId">
<?php
			if (empty($_GET['sensor_id'])) {
				$currentSensorId = 1;
			} else {
				$currentSensorId = $_GET['sensor_id'];
			}
			$rsSensors->data_seek(0);
			while ($rowSensors = $rsSensors->fetch_assoc()) {
?>
				<option value="<?php echo $rowSensors['id'] ?>" <?php echo $currentSensorId == $rowSensors['id'] ? 'selected' : ''; ?>><?php echo $rowSensors['name'] ?></option>
<?php
			}
?>				
			</select>
		</div>
	</form>
	<div id="highsharts_container" class="highsharts sensor" data-sensor-id="<?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] ? $_GET['sensor_id'] : '1'; ?>">
	
	</div>
<?php
		}
	}
?>
<br>
<div>
	<a href="index.php" class="btn btn-default">Zurück zur Übersicht</a>
</div>
<?php include "incs/html_foot.php" ?>