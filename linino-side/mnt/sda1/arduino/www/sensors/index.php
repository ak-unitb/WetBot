<?php
    require_once('../incs/db.php');
?>
<?php include "../incs/html_head.php" ?>

        <h1>Konfigurierte Sensoren</h1>

<?php
    if ($conn) {
?>
<?php
        $sql = "SELECT * FROM `sensors` ORDER BY `id`";

        $rs = $conn->query($sql);

        if ($rs === false) {
            trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
        } else {
?>
        <div class="table-responsive">
			<table class="table table-striped table-hover">
				<thead>
					<th>ID</th>
					<th>Name</th>
					<th>Beschreibung</th>
					<th>Sensor-Pin-Nummer</th>
					<th>Relais-Pin-Nummer</th>
					<th>Frequenz ab der der Sensor<br><i>zu trocken</i> meldet</th>
					<th>Frequenz ab der der Sensor<br><i>zu nass</i> meldet</th>
					<th>Aktiv?</th>
					<th></th>
				</thead>
				<tbody>
<?php
            $rs->data_seek(0);
            while ($row = $rs->fetch_assoc()) {
?>
					<tr>
						<td><?php echo $row['id'] ?></td>
						<td><?php echo $row['name'] ?></td>
						<td><?php echo $row['location_description'] ?></td>
						<td><?php echo $row['sensor_pin_number'] ?></td>
						<td><?php echo $row['relay_pin_number'] ?></td>
						<td><?php echo $row['frequency_threeshold_too_dry'] ?> Hz</td>
						<td><?php echo $row['frequency_threeshold_too_wet'] ?> Hz</td>
						<td><?php echo $row['active'] ? 'Ja' : 'Nein' ?></td>
						<td><a href="edit.php?id=<?php echo $row['id'] ?>">Bearbeiten</a></td>
					</tr>
<?php
            }
?>
				</tbody>
			</table>
        </div>
<?php
        }
    } else {
?>
        <p class="text-message bg-danger">Datenbank-Verbindung ist fehlgeschlagen.</p>
<?php
    }
?>
<?php include "../incs/html_foot.php" ?>