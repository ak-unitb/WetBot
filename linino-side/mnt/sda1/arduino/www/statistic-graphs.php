<?php
    require_once('incs/db.php');
?>
<?php include "incs/html_head.php" ?>

        <h1>Statistik</h1>
<?php
    if ($conn) {
?>
	<form action="statistic-graphs.php" method="get">
		<div class="form-group">
			<label for="name">Sensor</label>
			<select class="form-control" name="sensor_id" id="statisticsGraphSensorId">
				<option value="">-</option>
				<option value="1" <?php echo empty($_GET['sensor_id']) || $_GET['sensor_id'] == 1 ? 'selected' : ''; ?>>1</option>
				<option value="2" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 2 ? 'selected' : ''; ?>>2</option>
				<option value="3" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 3 ? 'selected' : ''; ?>>3</option>
				<option value="4" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 4 ? 'selected' : ''; ?>>4</option>
				<option value="5" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 5 ? 'selected' : ''; ?>>5</option>
				<option value="6" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 6 ? 'selected' : ''; ?>>6</option>
			</select>
		</div>
	</form>
	<div id="highsharts_container" class="highsharts sensor" data-sensor-id="<?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] ? $_GET['sensor_id'] : '1'; ?>">
	
	</div>
<?php
    }
?>
<br>
<div>
    <a href="index.php" class="btn btn-default">Zurück zur Übersicht</a>
</div>
<?php include "incs/html_foot.php" ?>