<?php
    require_once('incs/db.php');
?><!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>YunFreq - Current Value of Sensors</title>
    <meta name="description" content="YunFreq - Index">
    <link rel="stylesheet" media="all" href="assets/css/application.css">
    <script src="assets/js/application.js"></script>
  </head>
  <body>
    <header>
      <nav class="navbar navbar-inverse navbar-fixed-top">
        <div class="container">
          <div class="navbar-header">
            <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
              <span class="sr-only">Toggle navigation</span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
            </button>
            <a class="navbar-brand" href="/sd/">Home</a>
          </div>
          <div class="collapse navbar-collapse">
            <ul class="nav navbar-nav">
              <li><a href="#" onclick="alert('not implemented yet');">Sensors</a></li>
              <li><a href="./last-100.php">letzte hundert Einträge</a></li>
              <li><a href="/cgi-bin/luci/webpanel/homepage">Yun-Admin</a></li>
            </ul>
          </div>
        </div>
      </nav>

      </header>
        <main role="main">

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

      </main>

  </body>
</html>
