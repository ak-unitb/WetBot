<?php
	class Sensor {
		public $id,
		$name,
		$location_description,
		$sensor_pin_number,
		$relay_pin_number,
		$frequency_threeshold_too_dry,
		$frequency_threeshold_too_wet,
		$active;

		public function validate() {
			$errors = array();
			$warnings = array();
			if (empty($this->name)) {
				$errors['name'] = 'Name darf nicht leer sein!';
			}
			if (empty($this->frequency_threeshold_too_dry)) {
				$errors['frequency_threeshold_too_dry'] = 'Frequenz darf nicht leer sein!';
			} else if (!is_numeric($this->frequency_threeshold_too_dry)) {
				$errors['frequency_threeshold_too_dry'] = 'Frequenz muss ein numerischer Wert sein!';
			}
			if (empty($this->frequency_threeshold_too_wet)) {
				$errors['frequency_threeshold_too_wet'] = 'Frequenz darf nicht leer sein!';
			} else if (!is_numeric($this->frequency_threeshold_too_wet)) {
				$errors['frequency_threeshold_too_wet'] = 'Frequenz muss ein numerischer Wert sein!';
			}
			if (empty($this->location_description)) {
				$warnings['location_description'] = 'Beschreibung ist leer.';
			}
			return array('errors' => $errors, 'warnings' => $warnings);
		}

	}
?>