<?php
	require_once('../incs/db.php');
	
	if (!empty($_GET['reset-mcu']) && $_GET['reset-mcu'] == 'true') {
		$res = shell_exec('/usr/bin/reset-mcu');
		echo '<pre><code>';
		var_dump($res);
		echo '</code></pre>';
	}
	
?>
<?php include "../incs/html_head.php" ?>

		<h1>Sensoren</h1>

<?php
	if ($conn) {

		$sql = 'SELECT * FROM `sensors` WHERE `active` = TRUE ORDER BY `id`;';

		$rs = $conn->query($sql);

		if ($rs === false) {
			trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
		} else {
			if ($rs->num_rows <= 0) {
?>
		<p class="text-message bg-warning">Keine aktiven Sensoren<p>
<?php
			} else {
?>
		<div class="panel">
			<div class="table-responsive">
				<div class="panel-heading">
					<h2 class="panel-title">Aktive Sensoren</h2>
				</div>
				<table class="table table-striped table-hover rwd-table">
					<thead>
						<th>ID</th>
						<th>Name</th>
						<th>Beschreibung</th>
						<th>Sensor-Pin-Nummer</th>
						<th>Relais-Pin-Nummer</th>
						<th>Frequenz ab der der Sensor<br><i>zu trocken</i> meldet</th>
						<th>Frequenz ab der der Sensor<br><i>zu nass</i> meldet</th>
						<!--<th>Aktiv?</th>-->
						<th></th>
					</thead>
					<tbody>
<?php
				$rs->data_seek(0);
				while ($row = $rs->fetch_assoc()) {
?>
						<tr>
							<td data-th="ID"><?php echo $row['id'] ?></td>
							<td data-th="Name"><?php echo $row['name'] ?></td>
							<td data-th="Beschreibung"><?php echo $row['location_description'] ?></td>
							<td data-th="Sensor-Pin-Nummer"><?php echo $row['sensor_pin_number'] ?></td>
							<td data-th="Ralais-Pin-Nummer"><?php echo $row['relay_pin_number'] ?></td>
							<td data-th="zu trocken ab"><?php echo $row['frequency_threeshold_too_dry'] ?> Hz</td>
							<td data-th="zu nass unter"><?php echo $row['frequency_threeshold_too_wet'] ?> Hz</td>
							<!--<td data-th="Aktiv?"><?php echo $row['active'] ? 'Ja' : 'Nein' ?></td>-->
							<td data-th="Aktion"><a href="edit.php?id=<?php echo $row['id'] ?>">Bearbeiten</a></td>
						</tr>
<?php
				}
?>
					</tbody>
				</table>
			</div>
		</div>
<?php
			}
		}
?>
		<p>&nbsp;</p>
<?php
		$sql = 'SELECT * FROM `sensors` WHERE `active` = FALSE ORDER BY `id`;';

		$rs = $conn->query($sql);

		if ($rs === false) {
			trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
		} else {
			$rs->data_seek(0);
			if ($rs->num_rows <= 0) {
?>
		<p class="text-message bg-warning">Keine inaktiven Sensoren<p>
<?php
			} else {
?>
		<div class="panel">
			<div class="table-responsive">
				<div class="panel-heading">
					<h2 class="panel-title">Inaktive Sensoren</h2>
				</div>
				<table class="table table-striped table-hover rwd-table">
					<thead>
						<th>ID</th>
						<th>Name</th>
						<th>Beschreibung</th>
						<th>Sensor-Pin-Nummer</th>
						<th>Relais-Pin-Nummer</th>
						<th>Frequenz ab der der Sensor<br><i>zu trocken</i> meldet</th>
						<th>Frequenz ab der der Sensor<br><i>zu nass</i> meldet</th>
						<!--<th>Aktiv?</th>-->
						<th>Aktion</th>
					</thead>
					<tbody>
<?php
				$rs->data_seek(0);
				while ($row = $rs->fetch_assoc()) {
?>
						<tr>
							<td data-th="ID"><?php echo $row['id'] ?></td>
							<td data-th="Name"><?php echo $row['name'] ?></td>
							<td data-th="Beschreibung"><?php echo $row['location_description'] ?></td>
							<td data-th="Sensor-Pin-Nummer"><?php echo $row['sensor_pin_number'] ?></td>
							<td data-th="Ralais-Pin-Nummer"><?php echo $row['relay_pin_number'] ?></td>
							<td data-th="zu trocken ab"><?php echo $row['frequency_threeshold_too_dry'] ?> Hz</td>
							<td data-th="zu nass unter"><?php echo $row['frequency_threeshold_too_wet'] ?> Hz</td>
							<!--<td data-th="Aktiv?"><?php echo $row['active'] ? 'Ja' : 'Nein' ?></td>-->
							<td data-th="Aktion"><a href="edit.php?id=<?php echo $row['id'] ?>">Bearbeiten</a></td>
						</tr>
<?php
				}
?>
					</tbody>
				</table>
			</div>
		</div>
<?php
			}
		}
	} else {
?>
		<p class="text-message bg-danger">Datenbank-Verbindung ist fehlgeschlagen.</p>
<?php
	}
?>
<?php include "../incs/html_foot.php" ?>