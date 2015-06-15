<?php
    require_once('incs/db.php');
?>
<?php include "incs/html_head.php" ?>

          <h1>YunFreq - Current Value of Sensors</h1>
<?php
    if ($conn) {
?>
<?php
        $sqlSensors = "SELECT *  FROM `sensors`;";
        $rsSensors = $conn->query($sqlSensors);

        if ($rsSensors === false) {
            trigger_error('Wrong SQL: ' . $sqlSensors . ' Error: ' . $conn->error, E_USER_ERROR);
        } else {

            $rsSensors->data_seek(0);
            while ($rowSensors = $rsSensors->fetch_assoc()) {
                $sqlSensorDatas = "SELECT * FROM `sensor_datas` WHERE `sensor_id` = ".$rowSensors['id']." ORDER BY `created_at` DESC LIMIT 1";
                $rsSensorDatas = $conn->query($sqlSensorDatas);
                if ($rsSensorDatas === false) {
                    trigger_error('Wrong SQL: ' . $sqlSensorDatas . ' Error: ' . $conn->error, E_USER_ERROR);
                } else {
                    $rsSensorDatas->data_seek(0);
                    $rowSensorDatas = $rsSensorDatas->fetch_assoc();
?>
          <div class="sensor-data" data-id="<?php echo $rowSensors['id'] ?>" data-pin-number="<?php echo $rowSensors['pin_number'] ?>">
             <h2 class="sensor-data__name"><?php echo $rowSensors['name'] ?></h2>
             <p class="sensor-data__location-description">
                 <?php echo $rowSensors['location_desciption'] ?>
             </p>
             <div
               class="sensor-data__status sensor-data__status--grade-of-dryness-<?php echo $rowSensorDatas['grade_of_dryness'] ?>"
               data-gread-of-dryness="Trockenheitsgrad: <?php echo $rowSensorDatas['grade_of_dryness'] ?>"
               title="Created @ <?php echo $rowSensorDatas['created_at'] ?>">
               <span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--0">klatschnass</span>
               <span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--1">nass</span>
               <span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--2">feucht</span>
               <span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--3">trocken</span>
               <span class="sensor-data__status__grade-of-dryness sensor-data__status__grade-of-dryness--4">staubig</span>
               <span class="sensor-data__status__frequency">Frequenz: <?php echo $rowSensorDatas['frequency'] ?> Hz</span>
               <span class="sensor-data__status__comment"><?php if (!empty($rowSensorDatas['comment'])) { ?>Zusätzliche Information: <?php echo $rowSensorDatas['comment']; } else { ?><?php echo "&nbsp;"; } ?></span>
             </div>
          </div>
<?php
                }
            }
        }

    } else {
?>
		<h3 class="has-error">Database connection failed</h3>
<?php
    }
?>
<?php include "incs/html_foot.php" ?>
