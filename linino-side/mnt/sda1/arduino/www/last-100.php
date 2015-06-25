<?php
	require_once('incs/db.php');
?>
<?php include "incs/html_head.php" ?>

		<h1>Statistik</h1>
<?php
	if ($conn) {

                $sql = "SELECT * FROM `sensor_datas`";

                $whereClause = array();
                if (!empty($_GET['grade_of_dryness'])) {
                        $whereClause[] = " `grade_of_dryness` = " . intval($_GET['grade_of_dryness']) . "";
                }
                if (!empty($_GET['comment'])) {
                        $whereClause[] = " `comment` = '" . stripslashes($_GET['comment']) . "'";
                }
                if (!empty($_GET['sensor_id'])) {
                        $whereClause[] = " `sensor_id` = " . intval($_GET['sensor_id']) . "";
                }

                if (!empty($whereClause)) {
                        $sql .= " WHERE " . implode(' AND ', $whereClause) . "";
                }
                $countLastEnrties = 720;
                $sql .= " ORDER BY `created_at` DESC LIMIT " . $countLastEnrties;

                $rs = $conn->query($sql);
                if ($rs === false) {
                        trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
                } else {
                        $rs->data_seek(0);
                        if ($rs->num_rows <= 0) {
?>
		<p class="text-message bg-warning">Keine Datensätze gefunden. Bitte prüfen Sie ggf. die Filter-Angaben.<p>
<?php
			}
?>
		<h2>Liste der letzten <?php echo $countLastEnrties ?> Einträge</h2>
		<div class="panel filterable">
			<div class="table-responsive">
				<form action="" method="get">
					<div class="panel-heading">
						<h2 class="panel-title">&nbsp;</h2>
						<div class="pull-right">
							<button class="btn btn-default btn-xs btn-filter" type="button"><span class="glyphicon glyphicon-filter"></span> Filter</button>
						</div>
					</div>
					<table class="table table-striped table-hover rwd-table">
						<thead>
							<tr>
								<!--<th class="hidden-xs">ID</th>-->
								<th class="hidden-xs">Frequenz</th>
								<th>Grad der Trockenheit</th>
								<th class="hidden-xs">Kommentar</th>
								<th>Sensor-Name</th>
								<th>gespeichert am/um</th>
							</tr>
						</thead>
						<tbody>
							<tr class="filters">
								<!--<td></td>-->
								<td data-th="Filter"></td>
								<td data-th="Grad der Trockenheit">
									<select class="form-control" name="grade_of_dryness" disabled>
										<option value="">-</option>
										<option value="3" <?php echo !empty($_GET['grade_of_dryness']) && $_GET['grade_of_dryness'] == 3 ? 'selected' : ''; ?>>3</option>
										<option value="2" <?php echo !empty($_GET['grade_of_dryness']) && $_GET['grade_of_dryness'] == 2 ? 'selected' : ''; ?>>2</option>
										<option value="1" <?php echo !empty($_GET['grade_of_dryness']) && $_GET['grade_of_dryness'] == 1 ? 'selected' : ''; ?>>1</option>
									</select>
								</td>
								<td data-th="Kommentar"><input type="text" class="form-control" name="comment" placeholder="" value="<?php echo !empty($_GET['comment']) ? $_GET['comment'] : ''; ?>" disabled></td>
								<td data-th="Sensor-ID">
									<select class="form-control" name="sensor_id" disabled>
										<option value="">-</option>
<?php
                        $sqlSensors = 'SELECT *  FROM `sensors` WHERE `active` = TRUE;';
                        $rsSensors = $conn->query($sqlSensors);
                        $mapSensorIdName = array();

                        if ($rsSensors === false) {
                                trigger_error('Wrong SQL: ' . $sqlSensors . ' Error: ' . $conn->error, E_USER_ERROR);
                        } else {
                                $currentSensorId = $_GET['sensor_id'];
                                $rsSensors->data_seek(0);
                                while ($rowSensors = $rsSensors->fetch_assoc()) {
                                $mapSensorIdName[$rowSensors['id']] = $rowSensors['name'];
?>
                                                                                <option value="<?php echo $rowSensors['id'] ?>" <?php echo $currentSensorId == $ro
<?php
                                }
                        }
?>
									</select>
								</td>
								<td data-th="Aktion"><button type="submit" class="btn btn-primary pull-right">Filter anwenden</button></td>
							</tr>
<?php
			$rs->data_seek(0);
			while($row = $rs->fetch_assoc()){
?>
							<tr>
								<!--<td data-th="ID" class="hidden-xs"><?php echo $row['id'] ?></td>-->
								<td data-th="Frequenz" class="hidden-xs"><?php echo $row['frequency'] ?> Hz</td>
								<td data-th="Grad der Trockenheit"><?php echo $row['grade_of_dryness'] ?></td>
								<td data-th="Kommentar" class="hidden-xs"><?php echo $row['comment'] ?></td>
								<td data-th="Sensor-Name"><?php echo $mapSensorIdName[$row['sensor_id']] ?></td>
								<td data-th="am/um"><?php echo $row['created_at'] ?></td>
							</tr>
<?php
			}
		}
?>
						</thead>
					</table>
				</form>
			</div>
		</div>
<?php
	} else {
?>
		<p class="text-message has-error">Datenbank Verbindung fehlgeschlagen</p>
<?php
	}
?>

<?php include "incs/html_foot.php" ?>
