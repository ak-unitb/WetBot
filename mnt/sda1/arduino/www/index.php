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
?>
          <table border="1">
            <thead>
                <tr>
                    <th>ID</th>
                    <th>Pin Number</th>
                    <th>Name</th>
                    <th>Location Description</th>
                    <th>last Frequency</th>
                    <th>last Grade of Dryness</th>
                    <th>last Comment</th>
                    <th>Created @</th>
                </tr>
            </thead>
            <tbody>
<?php
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
                <tr>
                    <td><?php echo $rowSensors['id'] ?></td>
                    <td><?php echo $rowSensors['pin_number'] ?></td>
                    <td><?php echo $rowSensors['name'] ?></td>
                    <td><?php echo $rowSensors['location_desciption'] ?></td>
                    <td><?php echo $rowSensorDatas['frequency'] ?></td>
                    <td><?php echo $rowSensorDatas['grade_of_dryness'] ?></td>
                    <td><?php echo $rowSensorDatas['comment'] ?></td>
                    <td><?php echo $rowSensorDatas['created_at'] ?></td>
                </tr>
<?php
                }
            }
        }
?>
            </thead>
        </table>
<?php
    } else {
?>
		<h3 class="has-error">Database connection failed</h3>
<?php
    }
?>

      </main>

  </body>
</html>
