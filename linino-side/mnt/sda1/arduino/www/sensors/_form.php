<!--
  `name` VARCHAR(255) NOT NULL,
  `location_desciption` TEXT NULL DEFAULT NULL,
  `sensor_pin_number` VARCHAR(2) NOT NULL,
  `relay_pin_number` VARCHAR(2) NOT NULL,
  `frequency_threeshold_too_dry` INT(10) UNSIGNED NOT NULL DEFAULT 0,
  `frequency_threeshold_too_wet` INT(10) UNSIGNED NOT NULL DEFAULT 0,
-->
<div class="form-group">
    <label for="name">Name</label>
    <input type="text" class="form-control" id="name" placeholder="Name des Sensors" maxlength="255">
</div>
<div class="form-group">
    <label for="location_desciption">Beschreibung</label>
    <input type="text" class="form-control" id="location_desciption" placeholder="Beshchreibung des Sensors" maxlength="32000">
</div>
<div class="form-group">
    <label for="sensor_pin_number">Sensor Pin-Nummer</label>
    <input type="text" class="form-control" id="sensor_pin_number" placeholder="PIN-Nummer des Sensors (A0 - A5)" maxlength="2">
</div>
<div class="btn-group clearfix">
    <button type="button" class="form-control btn btn-default dropdown-toggle" data-toggle="dropdown">
        Sensor Pin-Nummer <span class="caret"></span>
    </button>
    <ul class="dropdown-menu" role="menu">
        <li><a href="#">A0</a></li>
        <li><a href="#">A1</a></li>
        <li><a href="#">A2</a></li>
    </ul>
</div>

<button type="submit" class="btn btn-primary">Speichern</button>