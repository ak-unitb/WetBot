<?php

    require_once('sensor.class.php');

	$saved = false;
	$errorMessage = "";

    $sensor = new Sensor();
	$validationResult = array('errors' => array(), 'warnings' => array());

    if ($_GET['id'] && !$_POST['id']) { // edit a sensor
		$sql = "SELECT * FROM `sensors` WHERE `id` = " . $_GET['id'];

        $rs = $conn->query($sql);

        if ($rs === false) {
            trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
        } else {
            $rs->data_seek(0);
            while ($row = $rs->fetch_assoc()) {
				$sensor->id = $row['id'];
				$sensor->name = $row['name'];
				$sensor->location_description = $row['location_description'];
				$sensor->sensor_pin_number = $row['sensor_pin_number'];
				$sensor->relay_pin_number = $row['relay_pin_number'];
				$sensor->frequency_threeshold_too_dry = $row['frequency_threeshold_too_dry'];
				$sensor->frequency_threeshold_too_wet = $row['frequency_threeshold_too_wet'];
				$sensor->active = $row['active'];
			}
		}
	} else if (!empty($_POST['id'])) { // request of saving a new sensor
		// validate and reject or save
		$sensor->id = $_POST['id'];
		$sensor->name = $_POST['name'];
		$sensor->location_description = $_POST['location_description'];
		$sensor->sensor_pin_number = $_POST['sensor_pin_number'];
		$sensor->relay_pin_number = $_POST['relay_pin_number'];
		$sensor->frequency_threeshold_too_dry = $_POST['frequency_threeshold_too_dry'];
		$sensor->frequency_threeshold_too_wet = $_POST['frequency_threeshold_too_wet'];
		$sensor->active = $_POST['active'];

		$validationResult = $sensor->validate();
		if (empty($validationResult['errors'])) {
			$sql = "UPDATE `sensors` SET `name` = '" . stripslashes($sensor->name) . "', `location_description` = '" . stripslashes($sensor->location_description) . "', `frequency_threeshold_too_dry` = " . intval($sensor->frequency_threeshold_too_dry) . ", `frequency_threeshold_too_wet` = " . intval($sensor->frequency_threeshold_too_wet) . ", `active` = " . $sensor->active . " WHERE `id` = " . $sensor->id . ";";
			$rs = $conn->query($sql);
			if ($rs === false) {
				// saving failed
				$errorMessage = 'Der Sensor konnte nicht gespeichert werden.';
			} else {
				$saved = true;
			}
		}
	}

?>

<pre><code>
<?php var_dump($sensor); ?><br>
<?php var_dump($validationResult['errors']); ?><br>
<?php var_dump($validationResult['warnings']); ?><br>
</code></pre>

<?php
	if ($saved) {
?>
    <p class="text-message bg-success">Der Sensor wurde gespeichert.</p>
<?php
	} else {
?>
<input type="hidden" name="id" value="<?php echo $sensor->id ?>">

<div class="form-group <?php echo (!empty($validationResult['errors']['name'])) ? 'has-error' : '' ?>">
    <label for="name">Name *</label>
    <input type="text" class="form-control" id="name" name="name" placeholder="Name des Sensors" value="<?php echo $sensor->name ?>" maxlength="255">
    <?php if (!empty($validationResult['errors']['name'])) { ?><span class="help-block"><?php echo $validationResult['errors']['name'] ?></span><?php } ?>
</div>
<div class="form-group <?php echo (!empty($validationResult['warnings']['location_description'])) ? 'has-warning' : '' ?>">
    <label for="location_description">Beschreibung</label>
    <input type="text" class="form-control" id="location_description" name="location_description" placeholder="Beschhreibung des Sensors" value="<?php echo $sensor->location_description ?>" maxlength="32000">
    <?php if (!empty($validationResult['warnings']['location_description'])) { ?><span class="help-block"><?php echo $validationResult['warnings']['location_description'] ?></span><?php } ?>
</div>
<div class="form-group">
    <label for="sensor_pin_number">Sensor Pin-Nummer</label>
    <select class="form-control" id="sensor_pin_number" name="sensor_pin_number" disabled>
        <option value="18" <?php if ($sensor->sensor_pin_number == '18') { echo 'selected'; } ?>>A0</option>
        <option value="19" <?php if ($sensor->sensor_pin_number == '19') { echo 'selected'; } ?>>A1</option>
        <option value="20" <?php if ($sensor->sensor_pin_number == '20') { echo 'selected'; } ?>>A2</option>
        <option value="21" <?php if ($sensor->sensor_pin_number == '21') { echo 'selected'; } ?>>A3</option>
        <option value="22" <?php if ($sensor->sensor_pin_number == '22') { echo 'selected'; } ?>>A4</option>
        <option value="23" <?php if ($sensor->sensor_pin_number == '23') { echo 'selected'; } ?>>A5</option>
    </select>
</div>
<div class="form-group">
    <label for="relay_pin_number">Ventil Pin-Nummer</label>
    <select class="form-control" id="relay_pin_number" name="relay_pin_number" disabled>
        <option value="4" <?php if ($sensor->relay_pin_number == '4') { echo 'selected'; } ?>>4</option>
        <option value="5" <?php if ($sensor->relay_pin_number == '5') { echo 'selected'; } ?>>5</option>
        <option value="6" <?php if ($sensor->relay_pin_number == '6') { echo 'selected'; } ?>>6</option>
        <option value="7" <?php if ($sensor->relay_pin_number == '7') { echo 'selected'; } ?>>7</option>
        <option value="8" <?php if ($sensor->relay_pin_number == '8') { echo 'selected'; } ?>>8</option>
        <option value="9" <?php if ($sensor->relay_pin_number == '9') { echo 'selected'; } ?>>9</option>
    </select>
</div>
<div class="form-group <?php echo (!empty($validationResult['errors']['frequency_threeshold_too_dry'])) ? 'has-error' : '' ?>">
    <label for="frequency_threeshold_too_dry">Frequenz ab der der Sensor <i>zu trocken</i> meldet *</label>
    <div class="input-group">
        <input type="text" class="form-control" id="frequency_threeshold_too_dry" name="frequency_threeshold_too_dry" placeholder="29629" value="<?php echo $sensor->frequency_threeshold_too_dry; ?>" maxlength="6">
        <span class="input-group-addon"> Hz</span>
    </div>
    <?php if (!empty($validationResult['errors']['frequency_threeshold_too_dry'])) { ?><span class="help-block"><?php echo $validationResult['errors']['frequency_threeshold_too_dry'] ?></span><?php } ?>
</div>
<div class="form-group <?php echo (!empty($validationResult['errors']['frequency_threeshold_too_wet'])) ? 'has-error' : '' ?>">
    <label for="frequency_threeshold_too_wet">Frequenz ab der der Sensor <i>zu nass</i> meldet *</label>
    <div class="input-group">
        <input type="text" class="form-control" id="frequency_threeshold_too_wet" name="frequency_threeshold_too_wet" placeholder="17000" value="<?php echo $sensor->frequency_threeshold_too_wet; ?>" maxlength="6">
        <span class="input-group-addon"> Hz</span>
    </div>
    <?php if (!empty($validationResult['errors']['frequency_threeshold_too_wet'])) { ?><span class="help-block"><?php echo $validationResult['errors']['frequency_threeshold_too_wet'] ?></span><?php } ?>
</div>
<div class="form-group">
    <label class="checkbox-inline">
        <input type="hidden" name="active" value="0">
        <input type="checkbox" name="active" value="1" <?php echo ($sensor->active) ? 'checked' : ''; ?>> Sensor ist aktiv
    </label>
</div>

<button type="submit" class="btn btn-primary pull-right">Speichern</button>

<?php
	}
?>