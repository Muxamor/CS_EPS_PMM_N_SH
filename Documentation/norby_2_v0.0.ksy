meta:
  id: norby
  file-extension: norby
  endian: le
seq:
  - id: header
    type: header
  - id: payload
    type:
      switch-on: header.msg_type_id
      cases:
        0: tmi0
        2: tmi0
        4: tmi1
        6: tmi2
        8: tmi3
        10: eps_tmi0
        24: tmi5
        26: tmi6
        28: eps_tmi1
        30: eps_tmi2
        32: eps_tmi3
        53: exp_frame
  - id: crc16
    type: u2
    if: header.msg_type_id != 53 and header.msg_type_id != 16 and header.msg_type_id != 14 and header.msg_type_id != -32752

instances:
  validate_data:
    pos: 15
    size: header.length - 16
    if: header.length > 15
  validate_method:
    value: '"crc16_x_25"'
  validate_value:
    value: crc16

  telemetry:
    value: payload
    if: header.msg_type_id == 0 or header.msg_type_id == 2 or header.msg_type_id == 4 or header.msg_type_id == 6 or header.msg_type_id == 8 or header.msg_type_id == 10 or header.msg_type_id == 24 or header.msg_type_id == 26 or header.msg_type_id == 28 or header.msg_type_id == 30 or header.msg_type_id == 53

types:
  header:
    seq:
      - id: length
        type: u1
      - id: receiver_address
        type: ip_address
      - id: transmitter_address
        type: ip_address
      - id: transaction_number
        type: u2be
      - size: 2
      - id: msg_type_id
        type: s2be

  tmi0:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: frame_definition
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: brk_title
        type: strz
        size: 24
        encoding: ASCII
      - id: brk_number_active
        type: u1
      - id: brk_restarts_count_active
        type: s4
      - id: brk_current_mode_id
        type: u1
      - id: brk_transmitter_power_active
        type: s1
      - id: brk_temp_active
        type: s1
      - id: brk_module_state_active
        size: 2
      - id: brk_voltage_offset_amplifier_active
        type: u2
      - id: brk_last_received_packet_rssi_active
        type: s1
      - id: brk_last_received_packet_snr_active
        type: s1
      - id: brk_archive_record_pointer
        type: u2
      - id: brk_last_received_packet_snr_inactive
        type: s1
      - id: ms_module_state
        size: 2
      - id: ms_payload_state
        size: 2
      - id: ms_temp
        type: s1
      - id: ms_pn_supply_state
        type: u1
      - id: adcs_altitude_glonass
        type: s4
      - id: adcs_latitude_glonass
        type: s4
      - id: adcs_longitude_glonass
        type: s4
      - id: adcs_date_time_glonass
        type: u4
      - id: adcs_magnetic_induction_module
        type: u2
      - id: adcs_angular_velocity_vector_avg
        type: vector
      - id: adcs_angle_priority1
        type: u2
      - id: adcs_angle_priority2
        type: u2
      - id: adcs_mk_temp_dsg1
        type: s1
      - id: adcs_mk_temp_dsg6
        type: s1
      - id: adcs_board_temp
        type: s1
      - id: adcs_state
        size: 2
      - id: adcs_state_dsg
        size: 6
      - id: adcs_orientation_number
        type: u1
      - id: eps_pmm_active_cpu
        type: u1
      - id: eps_pmm_temp
        type: s1
      - id: eps_power_main_bus_voltage
        type: u2
      - id: eps_cubesat_total_power_consuption
        type: u2
      - id: eps_cubesat_subsystem_power_consuption
        type: u2
      - id: eps_pam_total_generation_power
        type: u2
      - id: eps_pam_in_pwr_ch_state_id_bit_mask
        type: bit_mask_eps_pam_in_pwr_ch_state_id
      - id: eps_pam_median_temp_sp_1
        type: s1
      - id: eps_pam_median_temp_sp_2
        type: s1
      - id: eps_pam_median_temp_sp_3
        type: s1
      - id: eps_pam_median_temp_sp_4
        type: s1
      - id: eps_pam_median_temp_sp_5
        type: s1
      - id: eps_pam_median_temp_sp_6
        type: s1
      - id: eps_pbm_total_chrg_dchrg_pwr
        type: s2
      - id: eps_pbm_total_heat_pwr
        type: u2
      - id: eps_pbm_total_charge_level_mah
        type: u2
      - id: eps_pbm_heat_state_bit_mask
        type: bit_mask_eps_pbm_heat_state
      - id: eps_pbm_beacon_error_1_bit_mask
        type: bit_mask_eps_pbm_beacon_error_1
      - id: eps_pbm_beacon_error_2_bit_mask
        type: bit_mask_eps_pbm_beacon_error_2
      - id: eps_pbm_beacon_error_3_bit_mask
        type: bit_mask_eps_pbm_beacon_error_3
      - id: eps_beacon_error_1_bit_mask
        type: bit_mask_eps_beacon_error_1
      - id: eps_beacon_error_2_bit_mask
        type: bit_mask_eps_beacon_error_2
    instances:
      brk_last_received_packet_rssi_main:
        value: brk_last_received_packet_rssi_active
        if: _root.header.transmitter_address.str == "10.6.1.201" or _root.header.transmitter_address.str == "10.6.1.251"
      brk_last_received_packet_rssi_backup:
        value: brk_last_received_packet_rssi_active
        if: _root.header.transmitter_address.str == "10.6.1.202" or _root.header.transmitter_address.str == "10.6.1.252"

  tmi1:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: frame_definition
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: brk_synchronization_time
        size: 3
      - id: brk_number_active
        type: u1
      - id: brk_restarts_count_active
        type: u4
      - id: brk_current_mode_id
        type: u1
      - id: brk_transmitter_power_active
        type: s1
      - id: brk_temp_active
        type: s1
      - id: brk_temp_transmitter_active
        type: s1
      - id: brk_module_state_active
        size: 2
      - id: brk_voltage_offset_amplifier_active
        type: u2
      - id: brk_last_received_packet_rssi_active
        type: s1
      - id: brk_last_received_packet_snr_active
        type: s1
      - id: brk_module_read_timeout_eps
        type: u4
      - id: brk_module_read_timeout_adcs1
        type: u4
      - id: brk_module_read_timeout_adcs2
        type: u4
      - id: brk_module_read_timeout_ms
        type: u4
      - id: brk_additional_data_active
        type: u8
      - id: brk_restarts_count_inactive
        type: u4
      - id: brk_temp_inactive
        type: s1
      - id: brk_temp_transmitter_inactive
        type: s1
      - id: brk_module_state_inactive
        size: 2
      - id: brk_last_received_packet_rssi_inactive
        type: s1
      - id: brk_last_received_packet_snr_inactive
        type: s1
      - id: brk_additional_data_inactive
        type: u8
      - id: brk_forbidden_operations
        type: u1
      - id: ms_module_state
        size: 2
      - id: ms_pn_state1_1_a
        size: 2
      - id: ms_pn_state1_1_b
        size: 2
      - id: ms_pn_state1_2
        size: 2
      - id: ms_pn_state2_0
        size: 2
      - id: ms_decor_state
        size: 2
      - id: ms_voltage
        type: s1
      - id: ms_current
        type: s1
      - id: ms_pn_voltage1_1_a
        type: s1
      - id: ms_pn_current1_1_a
        type: s1
      - id: ms_pn_voltage1_1_b
        type: s1
      - id: ms_pn_current1_1_b
        type: s1
      - id: ms_pn_voltage1_2
        type: s1
      - id: ms_pn_current1_2
        type: s1
      - id: ms_pn_voltage2_0
        type: s1
      - id: ms_pn_current2_0
        type: s1
      - id: ms_voltage1_decor
        type: s1
      - id: ms_current1_decor
        type: s1
      - id: ms_voltage2_decor
        type: s1
      - id: ms_current2_decor
        type: s1
      - id: ms_temp
        type: s1
      - id: ms_pn_temp1_1_a
        type: s1
      - id: ms_pn_temp1_1_b
        type: s1
      - id: ms_pn_temp1_2
        type: s1
      - id: ms_pn_temp2_0
        type: s1
      - id: ms_pn_supply_state
        type: u1
      - id: ms_memory_iss
        type: u1
      - id: ms_memory_decor
        type: u1
      - id: ms_switch_on_count
        type: u1
      - id: ms_pn_supply_control
        type: u1
      - id: ms_additional_data1
        size: 2
      - id: ms_pn_memory_reading_pointer_iss
        type: s2
      - id: ms_pn_memory_writing_pointer_iss
        type: s2
      - id: ms_pn_memory_size_iss
        type: s2
      - id: ms_pn_memory_reading_pointer_decor
        type: s2
      - id: ms_pn_memory_writing_pointer_decor
        type: s2
      - id: ms_pn_memory_size_decor
        type: s2
      - id: ms_additional_data2
        size: 5
    instances:
      brk_last_received_packet_rssi_main:
        value: '_root.header.transmitter_address.str == "10.6.1.201" or _root.header.transmitter_address.str == "10.6.1.251" ? brk_last_received_packet_rssi_active :  brk_last_received_packet_rssi_inactive'
      brk_last_received_packet_rssi_backup:
        value: '_root.header.transmitter_address.str == "10.6.1.202" or _root.header.transmitter_address.str == "10.6.1.252" ? brk_last_received_packet_rssi_active :  brk_last_received_packet_rssi_inactive'

  tmi2:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: frame_definition
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: adcs
        type:
          switch-on: adcs_fw_ver
          cases:
            0: adcs_tmi2_v14
            17: adcs_tmi2_v17
            _: adcs_tmi2_v17
    instances:
      adcs_fw_ver:
        pos: 125
        type: u1

  adcs_tmi2_v14:
    seq:
      - id: altitude_glonass
        type: s4
      - id: latitude_glonass
        type: s4
      - id: longitude_glonass
        type: s4
      - id: horizontal_speed
        type: s4
      - id: horizontal_speed_azimuth
        type: s4
      - id: vertical_speed
        type: s4
      - id: date_time_glonass
        type: u4
      - id: is_data_valid_glonass
        type: u1
      - id: sun_visibility
        type: u1
      - id: restarts_count_active
        type: u4
      - id: acceleration_module
        type: u2
      - id: magnit_induction_vector_dsg1
        size: 3
      - id: magnit_induction_vector_dsg2
        size: 3
      - id: magnit_induction_vector_dsg3
        size: 3
      - id: magnit_induction_vector_dsg4
        size: 3
      - id: magnit_induction_vector_dsg5
        size: 3
      - id: magnit_induction_vector_dsg6
        size: 3
      - id: magnit_induction_vector
        size: 3
      - id: angular_velocity_vector_dsg1
        size: 3
      - id: angular_velocity_vector_dsg2
        size: 3
      - id: angular_velocity_vector_dsg3
        size: 3
      - id: angular_velocity_vector_dsg4
        size: 3
      - id: angular_velocity_vector_dsg5
        size: 3
      - id: angular_velocity_vector_dsg6
        size: 3
      - id: angular_velocity_vector
        size: 3
      - id: angle_priority1
        type: u2
      - id: angle_priority2
        type: u2
      - id: mk_temp_dsg1
        type: s1
      - id: mk_temp_dsg2
        type: s1
      - id: mk_temp_dsg3
        type: s1
      - id: mk_temp_dsg4
        type: s1
      - id: mk_temp_dsg5
        type: s1
      - id: mk_temp_dsg6
        type: s1
      - id: mk_temp
        type: s1
      - id: median_temp_dsg1
        type: s1
      - id: median_temp_dsg2
        type: s1
      - id: median_temp_dsg3
        type: s1
      - id: median_temp_dsg4
        type: s1
      - id: median_temp_dsg5
        type: s1
      - id: median_temp_dsg6
        type: s1
      - id: board_temp
        type: s1
      - id: state
        size: 2
      - id: state_dsg
        size: 6
      - id: advanced_data
        size: 7
      - id: stop_rotating_command_state
        type: u1
      - id: stop_rotation_accuracy
        type: u1
      - id: setup_orientation_command_state
        type: u1
      - id: orientation_number
        type: u1
      - id: orientation_setup_accuracy
        type: u1

  adcs_tmi2_v17:
    seq:
      - id: altitude_glonass
        type: s4
      - id: latitude_glonass
        type: s4
      - id: longitude_glonass
        type: s4
      - id: horizontal_speed
        type: s4
      - id: horizontal_speed_azimuth
        type: s4
      - id: vertical_speed
        type: s4
      - id: date_time_glonass
        type: u4
      - id: is_data_valid_glonass
        type: u1
      - id: sun_visibility
        type: u1
      - id: restarts_count_active
        type: u4
      - id: acceleration_module
        type: u2
      - id: magnit_induction_vector_dsg1
        size: 3
      - id: magnit_induction_vector_dsg2
        size: 3
      - id: magnit_induction_vector_dsg3
        size: 3
      - id: magnit_induction_vector_dsg4
        size: 3
      - id: magnit_induction_vector_dsg5
        size: 3
      - id: magnit_induction_vector_dsg6
        size: 3
      - id: magnit_induction_vector
        size: 3
      - id: angular_velocity_vector_dsg1
        size: 3
      - id: angular_velocity_vector_dsg2
        size: 3
      - id: angular_velocity_vector_dsg3
        size: 3
      - id: angular_velocity_vector_dsg4
        size: 3
      - id: angular_velocity_vector_dsg5
        size: 3
      - id: angular_velocity_vector_dsg6
        size: 3
      - id: angular_velocity_vector
        size: 3
      - id: angle_priority1
        type: u2
      - id: angle_priority2
        type: u2
      - id: mk_temp_dsg1
        type: s1
      - id: mk_temp_dsg2
        type: s1
      - id: mk_temp_dsg3
        type: s1
      - id: mk_temp_dsg4
        type: s1
      - id: mk_temp_dsg5
        type: s1
      - id: mk_temp_dsg6
        type: s1
      - id: mk_temp
        type: s1
      - id: median_temp_dsg1
        type: s1
      - id: median_temp_dsg2
        type: s1
      - id: median_temp_dsg3
        type: s1
      - id: median_temp_dsg4
        type: s1
      - id: median_temp_dsg5
        type: s1
      - id: median_temp_dsg6
        type: s1
      - id: board_temp
        type: s1
      - id: state
        size: 2
      - id: state_dsg
        size: 6
      - id: time_ms
        type: u4
      - id: advanced_data
        size: 3
      - id: stop_rotating_command_state
        type: u1
      - id: stop_rotation_accuracy
        type: u1
      - id: setup_orientation_accuracy
        type: u1
      - id: orientation_number
        type: u1
      - id: fw_ver
        type: u1

  tmi3:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: frame_definition
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: adcs
        type:
          switch-on: adcs_fw_ver
          cases:
            0: adcs_tmi3_v14
            17: adcs_tmi3_v17
            _: adcs_tmi3_v17
    instances:
      adcs_fw_ver:
        pos: 125
        type: u1

  adcs_tmi3_v14:
    seq:
      - id: estimate_rotating_stop_time
        type: u2
      - id: coil_current_x_positive
        type: u2
      - id: coil_current_x_negative
        type: u2
      - id: coil_current_y_positive
        type: u2
      - id: coil_current_y_negative
        type: u2
      - id: coil_current_z_positive
        type: u2
      - id: coil_current_z_negative
        type: u2
      - id: angles_vector_sd1
        size: 2
      - id: angles_vector_sd2
        size: 2
      - id: angles_vector_sd3
        size: 2
      - id: angles_vector_sd4
        size: 2
      - id: angles_vector_sd5
        size: 2
      - id: angles_vector_sd6
        size: 2
      - id: solar_direction_vector_sat_coordinate_system
        size: 3
      - id: angles_vector_dg1
        size: 2
      - id: angles_vector_dg2
        size: 2
      - id: angles_vector_dg3
        size: 2
      - id: angles_vector_dg4
        size: 2
      - id: angles_vector_dg5
        size: 2
      - id: angles_vector_dg6
        size: 2
      - id: earth_direction_vector_sat_coordinate_system
        size: 3
      - id: additional_data2
        size: 15
      - id: target_vector1
        size: 3
      - id: sat_vector1
        size: 3
      - id: target_vector2
        size: 3
      - id: sat_vector2
        size: 3
      - id: set_rotation_state
        type: u1
      - id: angular_velocity_x
        type: s1
      - id: angular_velocity_y
        type: s1
      - id: angular_velocity_z
        type: s1
      - id: matrix_settings_sd
        size: 16
      - id: photo_settings
        size: 10
      - id: make_photo_command_state
        type: u1
      - id: cameras_to_make_photo
        type: u1
      - id: make_sensor_sd_command_state
        type: u1
      - id: sd_to_make_snapshot
        size: 2
      - id: make_snapshot_dg_state
        type: u1
      - id: dg_to_make_snapshot
        size: 2
      - id: delete_data_command_stat
        type: u1

  adcs_tmi3_v17:
    seq:
      - id: estimate_rotating_stop_time
        type: u2
      - id: coil_current_x_positive
        type: u2
      - id: coil_current_x_negative
        type: u2
      - id: coil_current_y_positive
        type: u2
      - id: coil_current_y_negative
        type: u2
      - id: coil_current_z_positive
        type: u2
      - id: coil_current_z_negative
        type: u2
      - id: angles_vector_sd1
        type: u2
      - id: angles_vector_sd2
        type: u2
      - id: angles_vector_sd3
        type: u2
      - id: angles_vector_sd4
        type: u2
      - id: angles_vector_sd5
        type: u2
      - id: angles_vector_sd6
        type: u2
      - id: solar_direction_vector_sat_coordinate_system
        size: 3
      - id: angles_vector_dg1
        size: 2
      - id: angles_vector_dg2
        size: 2
      - id: angles_vector_dg3
        size: 2
      - id: angles_vector_dg4
        size: 2
      - id: angles_vector_dg5
        size: 2
      - id: angles_vector_dg6
        size: 2
      - id: earth_direction_vector_sat_coordinate_system
        size: 6
      - id: time_ms
        type: u4
      - id: additional_data2
        size: 11
      - id: target_vector1
        size: 3
      - id: sat_vector1
        size: 3
      - id: target_vector2
        size: 3
      - id: sat_vector2
        size: 3
      - id: set_rotation_state
        type: u1
      - id: angular_velocity_x
        type: s1
      - id: angular_velocity_y
        type: s1
      - id: angular_velocity_z
        type: s1
      - id: matrix_settings_sd
        size: 16
      - id: photo_settings
        size: 10
      - id: make_photo_command_state
        type: u1
      - id: cameras_to_make_photo
        type: u1
      - id: make_sensor_sd_command_state
        type: u1
      - id: sd_to_make_snapshot
        size: 2
      - id: make_snapshot_dg_state
        type: u1
      - id: dg_to_make_snapshot
        size: 2
      - id: fw_ver
        type: u1

  tmi5:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: frame_definition
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: adcs
        type:
          switch-on: adcs_fw_ver
          cases:
            0: adcs_tmi5_v14
            17: adcs_tmi5_v17
            _: adcs_tmi5_v17
    instances:
      adcs_fw_ver:
        pos: 125
        type: u1

  adcs_tmi5_v14:
    seq:
      - id: altitude_glonass_full
        type: s8
      - id: latitude_glonass_full
        type: s8
      - id: longitude_glonass_full
        type: s8
      - id: horizontal_speed_full
        type: s8
      - id: horizontal_speed_azimuth_full
        type: s8
      - id: vertical_speed_full
        type: s8
      - size: 7
      - id: is_data_valid_glonass
        type: u1
      - id: angular_velocity_vector_dsg1_full
        type: vector
      - id: angular_velocity_vector_dsg2_full
        type: vector
      - id: angular_velocity_vector_dsg3_full
        type: vector
      - id: angular_velocity_vector_dsg4_full
        type: vector
      - id: angular_velocity_vector_dsg5_full
        type: vector
      - id: angular_velocity_vector_dsg6_full
        type: vector
      - id: angular_velocity_vector_full
        type: vector
      - id: additional_data3
        size: 18

  adcs_tmi5_v17:
    seq:
      - id: altitude_glonass_full
        type: f8
      - id: latitude_glonass_full
        type: f8
      - id: longitude_glonass_full
        type: f8
      - id: horizontal_speed_full
        type: f8
      - id: horizontal_speed_azimuth_full
        type: f8
      - id: vertical_speed_full
        type: f8
      - id: date_time_glonass
        size: 7
      - id: is_data_valid_glonass
        type: u1
      - id: acceleration_vector_dsg1
        type: vector
      - id: acceleration_vector_dsg2
        type: vector
      - id: acceleration_vector_dsg3
        type: vector
      - id: acceleration_vector_dsg4
        type: vector
      - id: acceleration_vector_dsg5
        type: vector
      - id: acceleration_vector_dsg6
        type: vector
      - id: acceleration_vector
        type: vector
      - id: mk_temp
        type: s1
      - id: dtd_temp
        type: s1
      - id: dus_temp
        type: s1
      - id: mag_temp
        type: s1
      - id: time_ms
        type: u4
      - id: additional_data3
        size: 9
      - id: fw_ver
        type: u1

  tmi6:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: frame_definition
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: adcs
        type:
          switch-on: adcs_fw_ver
          cases:
            0: adcs_tmi6_v14
            17: adcs_tmi6_v17
            _: adcs_tmi6_v17
    instances:
      adcs_fw_ver:
        pos: 125
        type: u1

  adcs_tmi6_v14:
    seq:
      - id: acceleration_vector_dsg1
        type: vector
      - id: acceleration_vector_dsg2
        type: vector
      - id: acceleration_vector_dsg3
        type: vector
      - id: acceleration_vector_dsg4
        type: vector
      - id: acceleration_vector_dsg5
        type: vector
      - id: acceleration_vector_dsg6
        type: vector
      - id: acceleration_vector
        type: vector
      - id: magnit_induction_vector_dsg1_full
        type: vector
      - id: magnit_induction_vector_dsg2_full
        type: vector
      - id: magnit_induction_vector_dsg3_full
        type: vector
      - id: magnit_induction_vector_dsg4_full
        type: vector
      - id: magnit_induction_vector_dsg5_full
        type: vector
      - id: magnit_induction_vector_dsg6_full
        type: vector
      - id: magnetic_induction_module_full
        type: vector
      - id: board_temp_dsg1_sensor1
        type: s1
      - id: board_temp_dsg1_sensor2
        type: s1
      - id: board_temp_dsg1_sensor3
        type: s1
      - id: board_temp_dsg1_sensor4
        type: s1
      - id: board_temp_dsg2_sensor1
        type: s1
      - id: board_temp_dsg2_sensor2
        type: s1
      - id: board_temp_dsg2_sensor3
        type: s1
      - id: board_temp_dsg2_sensor4
        type: s1
      - id: board_temp_dsg3_sensor1
        type: s1
      - id: board_temp_dsg3_sensor2
        type: s1
      - id: board_temp_dsg3_sensor3
        type: s1
      - id: board_temp_dsg3_sensor4
        type: s1
      - id: board_temp_dsg4_sensor1
        type: s1
      - id: board_temp_dsg4_sensor2
        type: s1
      - id: board_temp_dsg4_sensor3
        type: s1
      - id: board_temp_dsg4_sensor4
        type: s1
      - id: board_temp_dsg5_sensor1
        type: s1
      - id: board_temp_dsg5_sensor2
        type: s1
      - id: board_temp_dsg5_sensor3
        type: s1
      - id: board_temp_dsg5_sensor4
        type: s1
      - id: board_temp_dsg6_sensor1
        type: s1
      - id: board_temp_dsg6_sensor2
        type: s1
      - id: board_temp_dsg6_sensor3
        type: s1
      - id: board_temp_dsg6_sensor4
        type: s1
      - id: board_temp_sensor1
        type: s1
      - id: board_temp_sensor2
        type: s1
      - id: board_temp_sensor3
        type: s1
      - id: board_temp_sensor4
        type: s1
      - id: additional_data3
        size: 4

  adcs_tmi6_v17:
    seq:
      - id: angular_velocity_vector_dsg1_full
        type: vector
      - id: angular_velocity_vector_dsg2_full
        type: vector
      - id: angular_velocity_vector_dsg3_full
        type: vector
      - id: angular_velocity_vector_dsg4_full
        type: vector
      - id: angular_velocity_vector_dsg5_full
        type: vector
      - id: angular_velocity_vector_dsg6_full
        type: vector
      - id: angular_velocity_vector_full
        type: vector
      - id: magnit_induction_vector_dsg1_full
        type: vector
      - id: magnit_induction_vector_dsg2_full
        type: vector
      - id: magnit_induction_vector_dsg3_full
        type: vector
      - id: magnit_induction_vector_dsg4_full
        type: vector
      - id: magnit_induction_vector_dsg5_full
        type: vector
      - id: magnit_induction_vector_dsg6_full
        type: vector
      - id: magnit_induction_vector_full
        type: vector
      - id: angles_vector_sd1_full
        type: u4
      - id: angles_vector_sd2_full
        type: u4
      - id: angles_vector_sd3_full
        type: u4
      - id: angles_vector_sd4_full
        type: u4
      - id: angles_vector_sd5_full
        type: u4
      - id: angles_vector_sd6_full
        type: u4
      - id: time_ms
        type: u4
      - id: additional_data3
        size: 3
      - id: fw_ver
        type: u1

  eps_tmi0:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: sat_id
        type: u2
      - id: loc_id
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: eps_tmi0_version
        type: u2
      - id: eps_pmm_constants_mode
        type: u1
      - id: eps_pmm_eps_mode
        type: u1
      - id: eps_pmm_active_cpu
        type: u1
      - id: eps_pmm_turn_off_pasive_cpu
        type: u1
      - id: eps_pmm_temp
        type: s1
      - id: eps_pmm_pwr_ch_state_bit_mask
        type: bit_mask_eps_pmm_pwr_ch_state
      - id: eps_pmm_pwr_ch_pg_bit_mask
        type: bit_mask_eps_pmm_pwr_ch_pg
      - id: eps_pmm_error_bit_mask
        type: bit_mask_eps_pmm_error
      - id: eps_pmm_restart_counter_cpu_main
        type: u4
      - id: eps_pmm_restart_counter_cpu_backup
        type: u4
      - id: eps_pmm_voltage_v_bat_ch_1
        type: u2
      - id: eps_pmm_voltage_v_bat_ch_2
        type: u2
      - id: eps_pmm_average_10s_voltage_vbat_ch_1
        type: u2
      - id: eps_pmm_average_10s_voltage_vbat_ch_2
        type: u2
      - id: eps_pmm_current_v_bat_ch_1
        type: s2
      - id: eps_pmm_current_v_bat_ch_2
        type: s2
      - id: eps_pmm_average_10s_current_v_bat_ch_1
        type: s2
      - id: eps_pmm_average_10s_current_v_bat_ch_2
        type: s2
      - id: eps_pmm_own_current_consuption
        type: u2
      - id: eps_pmm_internal_voltage
        type: u2
      - id: eps_power_main_bus_voltage
        type: u2
      - id: eps_cubesat_total_power_consuption
        type: u2
      - id: eps_cubesat_subsystem_power_consuption
        type: u2
      - id: eps_deploy_bit_mask
        type: bit_mask_eps_pmm_deploy
      - id: eps_pmm_software_version_fw
        type: u2
      - id: eps_pdm_pwr_ch_state_bit_mask
        type: bit_mask_eps_pdm_pwr_ch_state
      - id: eps_pdm_pwr_ch_pg_bit_mask
        type: bit_mask_eps_pdm_pwr_ch_pg
      - id: eps_pdm_error_bit_mask
        type: bit_mask_eps_pdm_error
      - id: eps_pdm_temp_sensor_1
        type: s1
      - id: eps_pdm_temp_sensor_2
        type: s1
      - id: eps_pdm_temp_sensor_3
        type: s1
      - id: eps_pdm_temp_sensor_4
        type: s1
      - id: eps_pdm_median_temp
        type: s1
      - id: eps_pdm_voltage_power_ch_1
        type: u2
      - id: eps_pdm_voltage_power_ch_2
        type: u2
      - id: eps_pdm_voltage_power_ch_3
        type: u2
      - id: eps_pdm_voltage_power_ch_4
        type: u2
      - id: eps_pdm_voltage_power_ch_5
        type: u2
      - id: eps_pdm_voltage_power_ch_6
        type: u2
      - id: eps_pdm_average_10s_voltage_power_ch_1
        type: u2
      - id: eps_pdm_average_10s_voltage_power_ch_2
        type: u2
      - id: eps_pdm_average_10s_voltage_power_ch_3
        type: u2
      - id: eps_pdm_average_10s_voltage_power_ch_4
        type: u2
      - id: eps_pdm_average_10s_voltage_power_ch_5
        type: u2
      - id: eps_pdm_average_10s_voltage_power_ch_6
        type: u2
      - id: eps_pdm_current_power_ch_1
        type: s2
      - id: eps_pdm_current_power_ch_2
        type: s2
      - id: eps_pdm_current_power_ch_3
        type: s2
      - id: eps_pdm_current_power_ch_4
        type: s2
      - id: eps_pdm_current_power_ch_5
        type: s2
      - id: eps_pdm_current_power_ch_6
        type: s2
      - id: eps_pdm_average_10s_current_power_ch_1
        type: s2
      - id: eps_pdm_average_10s_current_power_ch_2
        type: s2
      - id: eps_pdm_average_10s_current_power_ch_3
        type: s2
      - id: eps_pdm_average_10s_current_power_ch_4
        type: s2
      - id: eps_pdm_average_10s_current_power_ch_5
        type: s2
      - id: eps_pdm_average_10s_current_power_ch_6
        type: s2
 
  eps_tmi1:
    seq: 
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: sat_id
        type: u2
      - id: loc_id
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: eps_tmi1_version
        type: u2
      - id: eps_pam_total_generation_power
        type: u2
      - id: eps_pam_pwr_ch_state_bit_mask
        type: bit_mask_eps_pam_pwr_ch_state
      - id: eps_pam_pwr_ch_pg_bit_mask
        type: bit_mask_eps_pam_pwr_ch_pg
      - id: eps_pam_error_bit_mask
        type: bit_mask_eps_pam_error
      - id: eps_pam_temp_sensor_1
        type: s1
      - id: eps_pam_temp_sensor_2
        type: s1
      - id: eps_pam_temp_sensor_3
        type: s1
      - id: eps_pam_temp_sensor_4
        type: s1
      - id: eps_pam_median_temp
        type: s1
      - id: eps_pam_in_pwr_ch_state_id_bit_mask
        type: bit_mask_eps_pam_in_pwr_ch_state_id
      - id: eps_pam_in_pwr_ch_error_pwr_mon_bit_mask
        type: bit_mask_eps_pam_in_pwr_ch_error_pwr_mon
      - id: eps_pam_pwr_ch_1_voltage
        type: u2
      - id: eps_pam_pwr_ch_2_voltage
        type: u2
      - id: eps_pam_pwr_ch_3_voltage
        type: u2
      - id: eps_pam_pwr_ch_4_voltage
        type: u2
      - id: eps_pam_pwr_ch_lt_1_voltage
        type: u2
      - id: eps_pam_pwr_ch_lt_2_voltage
        type: u2
      - id: eps_pam_pwr_ch_1_current
        type: u2
      - id: eps_pam_pwr_ch_2_current
        type: u2
      - id: eps_pam_pwr_ch_3_current
        type: u2
      - id: eps_pam_pwr_ch_4_current
        type: u2
      - id: eps_pam_pwr_ch_lt_1_current
        type: u2
      - id: eps_pam_pwr_ch_lt_2_current
        type: u2
      - id: eps_pam_sp_ch_1_state_bit_mask
        type: bit_mask_eps_pam_sp_ch_1_state
      - id: eps_pam_sp_ch_2_state_bit_mask
        type: bit_mask_eps_pam_sp_ch_2_state
      - id: eps_pam_sp_ch_3_state_bit_mask
        type: bit_mask_eps_pam_sp_ch_3_state
      - id: eps_pam_sp_ch_4_state_bit_mask
        type: bit_mask_eps_pam_sp_ch_4_state
      - id: eps_pam_sp_ch_5_state_bit_mask
        type: bit_mask_eps_pam_sp_ch_5_state
      - id: eps_pam_sp_ch_6_state_bit_mask
        type: bit_mask_eps_pam_sp_ch_6_state
      - id: eps_pam_temp_sp_1_sensor_1
        type: s1
      - id: eps_pam_temp_sp_1_sensor_2
        type: s1
      - id: eps_pam_temp_sp_1_sensor_3
        type: s1
      - id: eps_pam_temp_sp_1_sensor_4
        type: s1
      - id: eps_pam_temp_sp_2_sensor_1
        type: s1
      - id: eps_pam_temp_sp_2_sensor_2
        type: s1
      - id: eps_pam_temp_sp_2_sensor_3
        type: s1
      - id: eps_pam_temp_sp_2_sensor_4
        type: s1
      - id: eps_pam_temp_sp_3_sensor_1
        type: s1
      - id: eps_pam_temp_sp_3_sensor_2
        type: s1
      - id: eps_pam_temp_sp_3_sensor_3
        type: s1
      - id: eps_pam_temp_sp_3_sensor_4
        type: s1
      - id: eps_pam_temp_sp_4_sensor_1
        type: s1
      - id: eps_pam_temp_sp_4_sensor_2
        type: s1
      - id: eps_pam_temp_sp_4_sensor_3
        type: s1
      - id: eps_pam_temp_sp_4_sensor_4
        type: s1
      - id: eps_pam_temp_sp_5_sensor_1
        type: s1
      - id: eps_pam_temp_sp_5_sensor_2
        type: s1
      - id: eps_pam_temp_sp_5_sensor_3
        type: s1
      - id: eps_pam_temp_sp_5_sensor_4
        type: s1
      - id: eps_pam_temp_sp_6_sensor_1
        type: s1
      - id: eps_pam_temp_sp_6_sensor_2
        type: s1
      - id: eps_pam_temp_sp_6_sensor_3
        type: s1
      - id: eps_pam_temp_sp_6_sensor_4
        type: s1
      - id: eps_pam_median_temp_sp_1
        type: s1
      - id: eps_pam_median_temp_sp_2
        type: s1
      - id: eps_pam_median_temp_sp_3
        type: s1
      - id: eps_pam_median_temp_sp_4
        type: s1
      - id: eps_pam_median_temp_sp_5
        type: s1
      - id: eps_pam_median_temp_sp_6
        type: s1
      - id: eps_reserve_29_byte
        size: 29

  eps_tmi2:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: sat_id
        type: u2
      - id: loc_id
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: eps_tmi2_version
        type: u2
      - id: eps_pbm_total_chrg_dchrg_pwr
        type: s2
      - id: eps_pbm_total_heat_pwr
        type: u2
      - id: eps_pbm_total_charge_level_mah
        type: u2
      - id: eps_pbm_total_charge_level_perc
        type: u1
      - id: eps_pbm_chrg_dchrg_cmd_reg_bit_mask
        type: bit_mask_eps_pbm_chrg_dchrg_cmd_reg
      - id: eps_pbm_thermostat_cmd_reg_bit_mask
        type: bit_mask_eps_pbm_thermostat_cmd_reg
      - id: eps_pbm_heat_state_bit_mask
        type: bit_mask_eps_pbm_heat_state
      - id: eps_pbm_wreck_chrg_cmd_reg_bit_mask
        type: bit_mask_eps_pbm_wreck_chrg_cmd_reg
      - id: eps_pbm_auto_correct_cap_cmd_reg_bit_mask
        type: bit_mask_eps_pbm_auto_correct_cap_cmd_reg
      - id: eps_pbm_1_error_bit_mask
        type: bit_mask_eps_pbm_1_error
      - id: eps_pbm_2_error_bit_mask
        type: bit_mask_eps_pbm_2_error
      - id: eps_pbm_3_error_bit_mask
        type: bit_mask_eps_pbm_3_error
      - id: eps_pbm_4_error_bit_mask
        type: bit_mask_eps_pbm_4_error
      - id: eps_pbm_1_controller_1_error_bit_mask
        type: bit_mask_eps_pbm_1_controller_1_error
      - id: eps_pbm_1_controller_2_error_bit_mask
        type: bit_mask_eps_pbm_1_controller_2_error
      - id: eps_pbm_2_controller_1_error_bit_mask
        type: bit_mask_eps_pbm_2_controller_1_error
      - id: eps_pbm_2_controller_2_error_bit_mask
        type: bit_mask_eps_pbm_2_controller_2_error
      - id: eps_pbm_3_controller_1_error_bit_mask
        type: bit_mask_eps_pbm_3_controller_1_error
      - id: eps_pbm_3_controller_2_error_bit_mask
        type: bit_mask_eps_pbm_3_controller_2_error
      - id: eps_pbm_4_controller_1_error_bit_mask
        type: bit_mask_eps_pbm_4_controller_1_error
      - id: eps_pbm_4_controller_2_error_bit_mask
        type: bit_mask_eps_pbm_4_controller_2_error
      - id: eps_pbm_1_charge_level_perc_branch_1
        type: u1
      - id: eps_pbm_1_charge_level_perc_branch_2
        type: u1
      - id: eps_pbm_2_charge_level_perc_branch_1
        type: u1
      - id: eps_pbm_2_charge_level_perc_branch_2
        type: u1
      - id: eps_pbm_3_charge_level_perc_branch_1
        type: u1
      - id: eps_pbm_3_charge_level_perc_branch_2
        type: u1
      - id: eps_pbm_4_charge_level_perc_branch_1
        type: u1
      - id: eps_pbm_4_charge_level_perc_branch_2
        type: u1
      - id: eps_pbm_1_charge_level_mah_branch_1
        type: u2
      - id: eps_pbm_1_charge_level_mah_branch_2
        type: u2
      - id: eps_pbm_2_charge_level_mah_branch_1
        type: u2
      - id: eps_pbm_2_charge_level_mah_branch_2
        type: u2
      - id: eps_pbm_3_charge_level_mah_branch_1
        type: u2
      - id: eps_pbm_3_charge_level_mah_branch_2
        type: u2
      - id: eps_pbm_4_charge_level_mah_branch_1
        type: u2
      - id: eps_pbm_4_charge_level_mah_branch_2
        type: u2
      - id: eps_pbm_1_chrg_dchrg_current_branch_1
        type: s2
      - id: eps_pbm_1_chrg_dchrg_current_branch_2
        type: s2
      - id: eps_pbm_2_chrg_dchrg_current_branch_1
        type: s2
      - id: eps_pbm_2_chrg_dchrg_current_branch_2
        type: s2
      - id: eps_pbm_3_chrg_dchrg_current_branch_1
        type: s2
      - id: eps_pbm_3_chrg_dchrg_current_branch_2
        type: s2
      - id: eps_pbm_4_chrg_dchrg_current_branch_1
        type: s2
      - id: eps_pbm_4_chrg_dchrg_current_branch_2
        type: s2
      - id: eps_pbm_1_temperature_controller_1
        type: s1
      - id: eps_pbm_1_temperature_controller_2
        type: s1
      - id: eps_pbm_1_onboard_temp_sensor_1
        type: s1
      - id: eps_pbm_1_onboard_temp_sensor_2
        type: s1
      - id: eps_pbm_1_onboard_temp_sensor_3
        type: s1
      - id: eps_pbm_1_onboard_temp_sensor_4
        type: s1
      - id: eps_pbm_2_temperature_controller_1
        type: s1
      - id: eps_pbm_2_temperature_controller_2
        type: s1
      - id: eps_pbm_2_onboard_temp_sensor_1
        type: s1
      - id: eps_pbm_2_onboard_temp_sensor_2
        type: s1
      - id: eps_pbm_2_onboard_temp_sensor_3
        type: s1
      - id: eps_pbm_2_onboard_temp_sensor_4
        type: s1
      - id: eps_pbm_3_temperature_controller_1
        type: s1
      - id: eps_pbm_3_temperature_controller_2
        type: s1
      - id: eps_pbm_3_onboard_temp_sensor_1
        type: s1
      - id: eps_pbm_3_onboard_temp_sensor_2
        type: s1
      - id: eps_pbm_3_onboard_temp_sensor_3
        type: s1
      - id: eps_pbm_3_onboard_temp_sensor_4
        type: s1
      - id: eps_pbm_4_temperature_controller_1
        type: s1
      - id: eps_pbm_4_temperature_controller_2
        type: s1
      - id: eps_pbm_4_onboard_temp_sensor_1
        type: s1
      - id: eps_pbm_4_onboard_temp_sensor_2
        type: s1
      - id: eps_pbm_4_onboard_temp_sensor_3
        type: s1
      - id: eps_pbm_4_onboard_temp_sensor_4
        type: s1
      - id: eps_reserve_11_byte
        size: 11

  eps_tmi3:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: sat_id
        type: u2
      - id: loc_id
        type: u2
      - id: frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: eps_tmi3_version
        type: u2
      - id: eps_pbm_1_voltage_branch_1_battery_1
        type: u2
      - id: eps_pbm_1_voltage_branch_1_battery_2
        type: u2
      - id: eps_pbm_1_voltage_branch_2_battery_1
        type: u2
      - id: eps_pbm_1_voltage_branch_2_battery_2
        type: u2
      - id: eps_pbm_2_voltage_branch_1_battery_1
        type: u2
      - id: eps_pbm_2_voltage_branch_1_battery_2
        type: u2
      - id: eps_pbm_2_voltage_branch_2_battery_1
        type: u2
      - id: eps_pbm_2_voltage_branch_2_battery_2
        type: u2
      - id: eps_pbm_3_voltage_branch_1_battery_1
        type: u2
      - id: eps_pbm_3_voltage_branch_1_battery_2
        type: u2
      - id: eps_pbm_3_voltage_branch_2_battery_1
        type: u2
      - id: eps_pbm_3_voltage_branch_2_battery_2
        type: u2
      - id: eps_pbm_4_voltage_branch_1_battery_1
        type: u2
      - id: eps_pbm_4_voltage_branch_1_battery_2
        type: u2
      - id: eps_pbm_4_voltage_branch_2_battery_1
        type: u2
      - id: eps_pbm_4_voltage_branch_2_battery_2
        type: u2
      - id: eps_pbm_1_max_current_branch_1
        type: s2
      - id: eps_pbm_1_min_current_branch_1
        type: s2
      - id: eps_pbm_1_max_current_branch_2
        type: s2
      - id: eps_pbm_1_min_current_branch_2
        type: s2
      - id: eps_pbm_2_max_current_branch_1
        type: s2
      - id: eps_pbm_2_min_current_branch_1
        type: s2
      - id: eps_pbm_2_max_current_branch_2
        type: s2
      - id: eps_pbm_2_min_current_branch_2
        type: s2
      - id: eps_pbm_3_max_current_branch_1
        type: s2
      - id: eps_pbm_3_min_current_branch_1
        type: s2
      - id: eps_pbm_3_max_current_branch_2
        type: s2
      - id: eps_pbm_3_min_current_branch_2
        type: s2
      - id: eps_pbm_4_max_current_branch_1
        type: s2
      - id: eps_pbm_4_min_current_branch_1
        type: s2
      - id: eps_pbm_4_max_current_branch_2
        type: s2
      - id: eps_pbm_4_min_current_branch_2
        type: s2
      - id: eps_pbm_1_min_voltage_branch_1
        type: u2
      - id: eps_pbm_1_min_voltage_branch_2
        type: u2
      - id: eps_pbm_2_min_voltage_branch_1
        type: u2
      - id: eps_pbm_2_min_voltage_branch_2
        type: u2
      - id: eps_pbm_3_min_voltage_branch_1
        type: u2
      - id: eps_pbm_3_min_voltage_branch_2
        type: u2
      - id: eps_pbm_4_min_voltage_branch_1
        type: u2
      - id: eps_pbm_4_min_voltage_branch_2
        type: u2
      - id: eps_pbm_1_heat_current_branch_1
        type: s2
      - id: eps_pbm_1_heat_current_branch_2
        type: s2
      - id: eps_pbm_2_heat_current_branch_1
        type: s2
      - id: eps_pbm_2_heat_current_branch_2
        type: s2
      - id: eps_pbm_3_heat_current_branch_1
        type: s2
      - id: eps_pbm_3_heat_current_branch_2
        type: s2
      - id: eps_pbm_4_heat_current_branch_1
        type: s2
      - id: eps_pbm_4_heat_current_branch_2
        type: s2
      - id: eps_pbm_1_age_branch_1
        type: u1
      - id: eps_pbm_1_age_branch_2
        type: u1
      - id: eps_pbm_2_age_branch_1
        type: u1
      - id: eps_pbm_2_age_branch_2
        type: u1
      - id: eps_pbm_3_age_branch_1
        type: u1
      - id: eps_pbm_3_age_branch_2
        type: u1
      - id: eps_pbm_4_age_branch_1
        type: u1
      - id: eps_pbm_4_age_branch_2
        type: u1
      - id: eps_pbm_1_chrg_dchrg_cycle_branch_1
        type: u1
      - id: eps_pbm_1_chrg_dchrg_cycle_branch_2
        type: u1
      - id: eps_pbm_2_chrg_dchrg_cycle_branch_1
        type: u1
      - id: eps_pbm_2_chrg_dchrg_cycle_branch_2
        type: u1
      - id: eps_pbm_3_chrg_dchrg_cycle_branch_1
        type: u1
      - id: eps_pbm_3_chrg_dchrg_cycle_branch_2
        type: u1
      - id: eps_pbm_4_chrg_dchrg_cycle_branch_1
        type: u1
      - id: eps_pbm_4_chrg_dchrg_cycle_branch_2
        type: u1

  exp_frame:
    seq:
      - id: frame_start_mark
        contents: [ 0xf1, 0x0f ]
      - id: exp_id
        contents: [ 14, 00 ]
      - id: exp_frame_number
        type: u2
      - id: frame_generation_time
        type: u4
      - id: adcs_altitude_glonass
        type: s4
      - id: adcs_latitude_glonass
        type: s4
      - id: adcs_longitude_glonass
        type: s4
      - id: adcs_horizontal_speed
        type: s4
      - id: adcs_horizontal_speed_azimuth
        type: s4
      - id: adcs_vertical_speed
        type: s4
      - id: adcs_date_time_glonass
        type: u4
      - id: brk_temp_transmitter_active
        type: s1
      - id: brk_temp_active
        type: s1

  bit_mask_eps_pmm_pwr_ch_state:
    seq:
      - id: reserve_4_bit
        type: b4
      - id: pmm_state_pwr_ch_i2c
        type: b1
      - id: pmm_state_pwr_ch_3v3
        type: b1
      - id: pmm_state_pwr_ch_5v
        type: b1
      - id: pmm_state_pwr_ch_deploy_pwr
        type: b1
      - id: pmm_state_pwr_ch_deploy_logic
        type: b1
      - id: pmm_state_pwr_ch_pbms_logic
        type: b1
      - id: pmm_state_pwr_ch_vbat_2_ef_2
        type: b1
      - id: pmm_state_pwr_ch_vbat_2_ef_1
        type: b1
      - id: pmm_state_pwr_ch_vbat_1_ef_2
        type: b1
      - id: pmm_state_pwr_ch_vbat_1_ef_1
        type: b1
      - id: pmm_state_pwr_ch_can_backup
        type: b1
      - id: pmm_state_pwr_ch_can_main
        type: b1

  bit_mask_eps_pmm_pwr_ch_pg:
    seq:
      - id: reserve_3_bit
        type: b3
      - id: pmm_pg_pwr_ch_i2c
        type: b1
      - id: pmm_pg_pwr_ch_3v3
        type: b1
      - id: pmm_pg_pwr_ch_5v
        type: b1
      - id: pmm_pg_pwr_ch_deploy_lp
        type: b1
      - id: pmm_pg_pwr_supply_backup
        type: b1
      - id: pmm_pg_pwr_supply_main
        type: b1
      - id: pmm_pg_pwr_ch_pbms_logic
        type: b1
      - id: pmm_pg_pwr_ch_vbat_2_ef_2
        type: b1
      - id: pmm_pg_pwr_ch_vbat_2_ef_1
        type: b1
      - id: pmm_pg_pwr_ch_vbat_1_ef_2
        type: b1
      - id: pmm_pg_pwr_ch_vbat_1_ef_1
        type: b1
      - id: pmm_pg_pwr_ch_can_backup
        type: b1
      - id: pmm_pg_pwr_ch_can_main
        type: b1

  bit_mask_eps_pmm_error:
    seq:
      - id: reserve_3_bit
        type: b3
      - id: pmm_error_can_port_b
        type: b1
      - id: pmm_error_can_port_m
        type: b1
      - id: pmm_error_uart_port_b
        type: b1
      - id: pmm_error_uart_port_m
        type: b1
      - id: pmm_error_fram_2
        type: b1
      - id: pmm_error_fram_1
        type: b1
      - id: pmm_error_i2c_deploy_adc
        type: b1
      - id: pmm_error_i2c_deploy_gpio
        type: b1
      - id: pmm_error_pwr_supply_m_b_curr_mon
        type: b1
      - id: pmm_error_i2c_gpio_2
        type: b1
      - id: pmm_error_i2c_gpio_1
        type: b1
      - id: pmm_error_i2c_gpio_1_0
        type: b1
      - id: pmm_error_pwr_mon_vbat_2_ef_2
        type: b1
      - id: pmm_error_pwr_mon_vbat_2_ef_1
        type: b1
      - id: pmm_error_pwr_mon_vbat_1_ef_2
        type: b1
      - id: pmm_error_pwr_mon_vbat_1_ef_1
        type: b1
      - id: pmm_error_pwr_ch_state_i2c
        type: b1
      - id: pmm_error_pwr_ch_state_3v3
        type: b1
      - id: pmm_error_pwr_ch_state_5v
        type: b1
      - id: pmm_error_pwr_ch_state_deploy_pwr
        type: b1
      - id: pmm_error_pwr_ch_state_deploy_logic
        type: b1
      - id: pmm_error_pwr_ch_state_pbms_logic
        type: b1
      - id: pmm_error_pwr_ch_state_vbat_2_ef_2
        type: b1
      - id: pmm_error_pwr_ch_state_vbat_2_ef_1
        type: b1
      - id: pmm_error_pwr_ch_state_vbat_1_ef_2
        type: b1
      - id: pmm_error_pwr_ch_state_vbat_1_ef_1
        type: b1
      - id: pmm_error_pwr_ch_state_can_backup
        type: b1
      - id: pmm_error_pwr_ch_state_can_main
        type: b1
      - id: pmm_temp_sensor_error
        type: b1
  
  bit_mask_eps_pmm_deploy:
    seq:
      - id: pmm_deploy_stage
        type: b3
      - id: reserve_3_bit
        type: b3
      - id: pmm_deploy_ch_2_z_n_2
        type: b1
      - id: pmm_deploy_ch_2_z_n_1
        type: b1
      - id: pmm_deploy_ch_1_z_p_2
        type: b1
      - id: pmm_deploy_ch_1_z_p_1
        type: b1
      - id: pmm_deploy_ch_3_y_n_2
        type: b1
      - id: pmm_deploy_ch_3_y_n_1
        type: b1
      - id: pmm_deploy_ch_4_y_p_2
        type: b1
      - id: pmm_deploy_ch_4_y_p_1
        type: b1
      - id: pmm_deploy_exit_2
        type: b1
      - id: pmm_deploy_exit_1
        type: b1
  
  bit_mask_eps_pdm_pwr_ch_state:
    seq:
      - id: reserve_4_bit
        type: b4
      - id: pdm_state_pwr_ch_ef_out_6
        type: b1
      - id: pdm_state_pwr_ch_ef_in_6
        type: b1
      - id: pdm_state_pwr_ch_ef_out_5
        type: b1
      - id: pdm_state_pwr_ch_ef_in_5
        type: b1
      - id: pdm_state_pwr_ch_ef_out_4
        type: b1
      - id: pdm_state_pwr_ch_ef_in_4
        type: b1
      - id: pdm_state_pwr_ch_ef_out_3
        type: b1
      - id: pdm_state_pwr_ch_ef_in_3
        type: b1
      - id: pdm_state_pwr_ch_ef_out_2
        type: b1
      - id: pdm_state_pwr_ch_ef_in_2
        type: b1
      - id: pdm_state_pwr_ch_ef_out_1
        type: b1
      - id: pdm_state_pwr_ch_ef_in_1
        type: b1
  
  bit_mask_eps_pdm_pwr_ch_pg:
      seq:
      - id: reserve_4_bit
        type: b4
      - id: pdm_pg_pwr_ch_ef_out_6
        type: b1
      - id: pdm_pg_pwr_ch_ef_in_6
        type: b1
      - id: pdm_pg_pwr_ch_ef_out_5
        type: b1
      - id: pdm_pg_pwr_ch_ef_in_5
        type: b1
      - id: pdm_pg_pwr_ch_ef_out_4
        type: b1
      - id: pdm_pg_pwr_ch_ef_in_4
        type: b1
      - id: pdm_pg_pwr_ch_ef_out_3
        type: b1
      - id: pdm_pg_pwr_ch_ef_in_3
        type: b1
      - id: pdm_pg_pwr_ch_ef_out_2
        type: b1
      - id: pdm_pg_pwr_ch_ef_in_2
        type: b1
      - id: pdm_pg_pwr_ch_ef_out_1
        type: b1
      - id: pdm_pg_pwr_ch_ef_in_1
        type: b1
  
  bit_mask_eps_pdm_error:
      seq:
      - id: reserve_7_bit
        type: b7
      - id: pdm_error_pwr_ch_pwr_mon_6
        type: b1
      - id: pdm_error_pwr_ch_state_ef_out_6
        type: b1
      - id: pdm_error_pwr_ch_state_ef_in_6
        type: b1
      - id: pdm_error_pwr_ch_pwr_mon_5
        type: b1
      - id: pdm_error_pwr_ch_state_ef_out_5
        type: b1
      - id: pdm_error_pwr_ch_state_ef_in_5
        type: b1
      - id: pdm_error_pwr_ch_pwr_mon_4
        type: b1
      - id: pdm_error_pwr_ch_state_ef_out_4
        type: b1
      - id: pdm_error_pwr_ch_state_ef_in_4
        type: b1
      - id: pdm_error_pwr_ch_pwr_mon_3
        type: b1
      - id: pdm_error_pwr_ch_state_ef_out_3
        type: b1
      - id: pdm_error_pwr_ch_state_ef_in_3
        type: b1
      - id: pdm_error_pwr_ch_pwr_mon_2
        type: b1
      - id: pdm_error_pwr_ch_state_ef_out_2
        type: b1
      - id: pdm_error_pwr_ch_state_ef_in_2
        type: b1
      - id: pdm_error_pwr_ch_pwr_mon_1
        type: b1
      - id: pdm_error_pwr_ch_state_ef_out_1
        type: b1
      - id: pdm_error_pwr_ch_state_ef_in_1
        type: b1
      - id: pdm_error_temp_sensor_4
        type: b1
      - id: pdm_error_temp_sensor_3
        type: b1
      - id: pdm_error_temp_sensor_2
        type: b1
      - id: pdm_error_temp_sensor_1
        type: b1
      - id: pdm_error_i2c_mux
        type: b1
      - id: pdm_error_i2c_gpio_2
        type: b1
      - id: pdm_error_i2c_gpio_1
        type: b1
  
  bit_mask_eps_pam_pwr_ch_state:
    seq:
      - id: reserve_8_bit
        type: b8
      - id: pam_state_pwr_ch_ef_6
        type: b1
      - id: pam_state_pwr_ch_ef_5
        type: b1
      - id: pam_state_pwr_ch_ef_4
        type: b1
      - id: pam_state_pwr_ch_ef_3
        type: b1
      - id: pam_state_pwr_ch_ef_2
        type: b1
      - id: pam_state_pwr_ch_ef_1
        type: b1
      - id: pam_state_ldo
        type: b1
      - id: pam_state_dc_dc
        type: b1

  bit_mask_eps_pam_pwr_ch_pg:
    seq:
      - id: reserve_8_bit
        type: b8
      - id: pam_pg_pwr_ch_ef_6
        type: b1
      - id: pam_pg_pwr_ch_ef_5
        type: b1
      - id: pam_pg_pwr_ch_ef_4
        type: b1
      - id: pam_pg_pwr_ch_ef_3
        type: b1
      - id: pam_pg_pwr_ch_ef_2
        type: b1
      - id: pam_pg_pwr_ch_ef_1
        type: b1
      - id: pam_pg_pg_ldo
        type: b1
      - id: pam_pg_dc_dc
        type: b1

  bit_mask_eps_pam_error:
    seq:
      - id: reserve_17_bit
        type: b17
      - id: pam_error_pwr_ch_state_ef_6
        type: b1
      - id: pam_error_pwr_ch_state_ef_5
        type: b1
      - id: pam_error_pwr_ch_state_ef_4
        type: b1
      - id: pam_error_pwr_ch_state_ef_3
        type: b1
      - id: pam_error_pwr_ch_state_ef_2
        type: b1
      - id: pam_error_pwr_ch_state_ef_1
        type: b1
      - id: pam_error_temp_sensor_4
        type: b1
      - id: pam_error_temp_sensor_3
        type: b1
      - id: pam_error_temp_sensor_2
        type: b1
      - id: pam_error_temp_sensor_1
        type: b1
      - id: pam_error_i2c_mux_2
        type: b1
      - id: pam_error_i2c_mux_1
        type: b1
      - id: pam_error_i2c_gpio
        type: b1
      - id: pam_error_state_ldo
        type: b1
      - id: pam_error_state_dc_dc
        type: b1

  bit_mask_eps_pam_in_pwr_ch_state_id:
    seq:
      - id: reserve_2_bit
        type: b2
      - id: pam_state_in_pwr_ch_id_6
        type: b1
      - id: pam_state_in_pwr_ch_id_5
        type: b1
      - id: pam_state_in_pwr_ch_id_4
        type: b1
      - id: pam_state_in_pwr_ch_id_3
        type: b1
      - id: pam_state_in_pwr_ch_id_2
        type: b1
      - id: pam_state_in_pwr_ch_id_1
        type: b1

  bit_mask_eps_pam_in_pwr_ch_error_pwr_mon:
    seq:
      - id: reserve_2_bit
        type: b2
      - id: pam_error_in_pwr_ch_pwr_mon_6
        type: b1
      - id: pam_error_in_pwr_ch_pwr_mon_5
        type: b1
      - id: pam_error_in_pwr_ch_pwr_mon_4
        type: b1
      - id: pam_error_in_pwr_ch_pwr_mon_3
        type: b1
      - id: pam_error_in_pwr_ch_pwr_mon_2
        type: b1
      - id: pam_error_in_pwr_ch_pwr_mon_1
        type: b1
  
  bit_mask_eps_pam_sp_ch_1_state:
    seq:
      - id: reserve_4_bit
        type: b4
      - id: pam_sp_1_error_temp_sensor_4
        type: b1
      - id: pam_sp_1_error_temp_sensor_3
        type: b1
      - id: pam_sp_1_error_temp_sensor_2
        type: b1
      - id: pam_sp_1_error_temp_sensor_1
        type: b1
      - id: pam_sp_1_state_id_6
        type: b1
      - id: pam_sp_1_state_id_5
        type: b1
      - id: pam_sp_1_state_id_4
        type: b1
      - id: pam_sp_1_state_id_3
        type: b1
      - id: pam_sp_1_state_id_2
        type: b1
      - id: pam_sp_1_state_id_1
        type: b1
      - id: pam_sp_1_error_i2c_gpio_2
        type: b1
      - id: pam_sp_1_error_i2c_gpio_1
        type: b1
  
  bit_mask_eps_pam_sp_ch_2_state:
    seq:
      - id: reserve_4_bit
        type: b4
      - id: pam_sp_2_error_temp_sensor_4
        type: b1
      - id: pam_sp_2_error_temp_sensor_3
        type: b1
      - id: pam_sp_2_error_temp_sensor_2
        type: b1
      - id: pam_sp_2_error_temp_sensor_1
        type: b1
      - id: pam_sp_2_state_id_6
        type: b1
      - id: pam_sp_2_state_id_5
        type: b1
      - id: pam_sp_2_state_id_4
        type: b1
      - id: pam_sp_2_state_id_3
        type: b1
      - id: pam_sp_2_state_id_2
        type: b1
      - id: pam_sp_2_state_id_1
        type: b1
      - id: pam_sp_2_error_i2c_gpio_2
        type: b1
      - id: pam_sp_2_error_i2c_gpio_1
        type: b1
  
  bit_mask_eps_pam_sp_ch_3_state:
    seq:
      - id: reserve_4_bit
        type: b4
      - id: pam_sp_3_error_temp_sensor_4
        type: b1
      - id: pam_sp_3_error_temp_sensor_3
        type: b1
      - id: pam_sp_3_error_temp_sensor_2
        type: b1
      - id: pam_sp_3_error_temp_sensor_1
        type: b1
      - id: pam_sp_3_state_id_6
        type: b1
      - id: pam_sp_3_state_id_5
        type: b1
      - id: pam_sp_3_state_id_4
        type: b1
      - id: pam_sp_3_state_id_3
        type: b1
      - id: pam_sp_3_state_id_2
        type: b1
      - id: pam_sp_3_state_id_1
        type: b1
      - id: pam_sp_3_error_i2c_gpio_2
        type: b1
      - id: pam_sp_3_error_i2c_gpio_1
        type: b1
  
  bit_mask_eps_pam_sp_ch_4_state:
    seq:
      - id: reserve_4_bit
        type: b4
      - id: pam_sp_4_error_temp_sensor_4
        type: b1
      - id: pam_sp_4_error_temp_sensor_3
        type: b1
      - id: pam_sp_4_error_temp_sensor_2
        type: b1
      - id: pam_sp_4_error_temp_sensor_1
        type: b1
      - id: pam_sp_4_state_id_6
        type: b1
      - id: pam_sp_4_state_id_5
        type: b1
      - id: pam_sp_4_state_id_4
        type: b1
      - id: pam_sp_4_state_id_3
        type: b1
      - id: pam_sp_4_state_id_2
        type: b1
      - id: pam_sp_4_state_id_1
        type: b1
      - id: pam_sp_4_error_i2c_gpio_2
        type: b1
      - id: pam_sp_4_error_i2c_gpio_1
        type: b1
  
  bit_mask_eps_pam_sp_ch_5_state:
    seq:
      - id: reserve_4_bit
        type: b4
      - id: pam_sp_5_error_temp_sensor_4
        type: b1
      - id: pam_sp_5_error_temp_sensor_3
        type: b1
      - id: pam_sp_5_error_temp_sensor_2
        type: b1
      - id: pam_sp_5_error_temp_sensor_1
        type: b1
      - id: pam_sp_5_state_id_6
        type: b1
      - id: pam_sp_5_state_id_5
        type: b1
      - id: pam_sp_5_state_id_4
        type: b1
      - id: pam_sp_5_state_id_3
        type: b1
      - id: pam_sp_5_state_id_2
        type: b1
      - id: pam_sp_5_state_id_1
        type: b1
      - id: pam_sp_5_error_i2c_gpio_2
        type: b1
      - id: pam_sp_5_error_i2c_gpio_1
        type: b1
  
  bit_mask_eps_pam_sp_ch_6_state:
    seq:
      - id: reserve_4_bit
        type: b4
      - id: pam_sp_6_error_temp_sensor_4
        type: b1
      - id: pam_sp_6_error_temp_sensor_3
        type: b1
      - id: pam_sp_6_error_temp_sensor_2
        type: b1
      - id: pam_sp_6_error_temp_sensor_1
        type: b1
      - id: pam_sp_6_state_id_6
        type: b1
      - id: pam_sp_6_state_id_5
        type: b1
      - id: pam_sp_6_state_id_4
        type: b1
      - id: pam_sp_6_state_id_3
        type: b1
      - id: pam_sp_6_state_id_2
        type: b1
      - id: pam_sp_6_state_id_1
        type: b1
      - id: pam_sp_6_error_i2c_gpio_2
        type: b1
      - id: pam_sp_6_error_i2c_gpio_1
        type: b1

  bit_mask_eps_pbm_chrg_dchrg_cmd_reg:
    seq:
      - id: pbm_4_enable_cmd_dchg_branch_2
        type: b1
      - id: pbm_4_enable_cmd_chg_branch_2
        type: b1
      - id: pbm_4_enable_cmd_dchg_branch_1
        type: b1
      - id: pbm_4_enable_cmd_chg_branch_1
        type: b1
      - id: pbm_3_enable_cmd_dchg_branch_2
        type: b1
      - id: pbm_3_enable_cmd_chg_branch_2
        type: b1
      - id: pbm_3_enable_cmd_dchg_branch_1
        type: b1
      - id: pbm_3_enable_cmd_chg_branch_1
        type: b1
      - id: pbm_2_enable_cmd_dchg_branch_2
        type: b1
      - id: pbm_2_enable_cmd_chg_branch_2
        type: b1
      - id: pbm_2_enable_cmd_dchg_branch_1
        type: b1
      - id: pbm_2_enable_cmd_chg_branch_1
        type: b1
      - id: pbm_1_enable_cmd_dchg_branch_2
        type: b1
      - id: pbm_1_enable_cmd_chg_branch_2
        type: b1
      - id: pbm_1_enable_cmd_dchg_branch_1
        type: b1
      - id: pbm_1_enable_cmd_chg_branch_1
        type: b1

  bit_mask_eps_pbm_thermostat_cmd_reg:
    seq:
      - id: pbm_4_heat_cmd_branch_2
        type: b1
      - id: pbm_4_heat_cmd_branch_1
        type: b1
      - id: pbm_3_heat_cmd_branch_2
        type: b1
      - id: pbm_3_heat_cmd_branch_1
        type: b1
      - id: pbm_2_heat_cmd_branch_2
        type: b1
      - id: pbm_2_heat_cmd_branch_1
        type: b1
      - id: pbm_1_heat_cmd_branch_2
        type: b1
      - id: pbm_1_heat_cmd_branch_1
        type: b1
  
  bit_mask_eps_pbm_heat_state:
    seq:
      - id: pbm_4_heat_state_branch_2
        type: b1
      - id: pbm_4_heat_state_branch_1
        type: b1
      - id: pbm_3_heat_state_branch_2
        type: b1
      - id: pbm_3_heat_state_branch_1
        type: b1
      - id: pbm_2_heat_state_branch_2
        type: b1
      - id: pbm_2_heat_state_branch_1
        type: b1
      - id: pbm_1_heat_state_branch_2
        type: b1
      - id: pbm_1_heat_state_branch_1
        type: b1
  
  bit_mask_eps_pbm_wreck_chrg_cmd_reg:
    seq:
      - id: pbm_4_wreck_chrg_cmd_branch_2
        type: b1
      - id: pbm_4_wreck_chrg_cmd_branch_1
        type: b1
      - id: pbm_3_wreck_chrg_cmd_branch_2
        type: b1
      - id: pbm_3_wreck_chrg_cmd_branch_1
        type: b1
      - id: pbm_2_wreck_chrg_cmd_branch_2
        type: b1
      - id: pbm_2_wreck_chrg_cmd_branch_1
        type: b1
      - id: pbm_1_wreck_chrg_cmd_branch_2
        type: b1
      - id: pbm_1_wreck_chrg_cmd_branch_1
        type: b1
  
  bit_mask_eps_pbm_auto_correct_cap_cmd_reg:
    seq:
      - id: pbm_4_auto_correct_cap_cmd_branch_2
        type: b1
      - id: pbm_4_auto_correct_cap_cmd_branch_1
        type: b1
      - id: pbm_3_auto_correct_cap_cmd_branch_2
        type: b1
      - id: pbm_3_auto_correct_cap_cmd_branch_1
        type: b1
      - id: pbm_2_auto_correct_cap_cmd_branch_2
        type: b1
      - id: pbm_2_auto_correct_cap_cmd_branch_1
        type: b1
      - id: pbm_1_auto_correct_cap_cmd_branch_2
        type: b1
      - id: pbm_1_auto_correct_cap_cmd_branch_1
        type: b1

  bit_mask_eps_pbm_1_error:
    seq:
      - id: reserve_2_bit
        type: b2
      - id: pbm_1_error_wreck_chrg_branch_2
        type: b1
      - id: pbm_1_error_wreck_chrg_branch_1
        type: b1
      - id: pbm_1_error_heat_2_temp_sensor_2
        type: b1
      - id: pbm_1_error_heat_2_temp_sensor_1
        type: b1
      - id: pbm_1_error_heat_2_heat
        type: b1
      - id: pbm_1_error_heat_2_ina238
        type: b1
      - id: pbm_1_error_heat_1_temp_sensor_2
        type: b1
      - id: pbm_1_error_heat_1_temp_sensor_1
        type: b1
      - id: pbm_1_error_heat_1_heat
        type: b1
      - id: pbm_1_error_heat_1_ina238
        type: b1
      - id: pbm_1_zero_energy_flag
        type: b1
      - id: pbm_1_low_energy_flag
        type: b1
      - id: pbm_1_error_i2c_mux
        type: b1
      - id: pbm_1_error_pca9534
        type: b1

  bit_mask_eps_pbm_2_error:
    seq:
      - id: reserve_2_bit
        type: b2
      - id: pbm_2_error_wreck_chrg_branch_2
        type: b1
      - id: pbm_2_error_wreck_chrg_branch_1
        type: b1
      - id: pbm_2_error_heat_2_temp_sensor_2
        type: b1
      - id: pbm_2_error_heat_2_temp_sensor_1
        type: b1
      - id: pbm_2_error_heat_2_heat
        type: b1
      - id: pbm_2_error_heat_2_ina238
        type: b1
      - id: pbm_2_error_heat_1_temp_sensor_2
        type: b1
      - id: pbm_2_error_heat_1_temp_sensor_1
        type: b1
      - id: pbm_2_error_heat_1_heat
        type: b1
      - id: pbm_2_error_heat_1_ina238
        type: b1
      - id: pbm_2_zero_energy_flag
        type: b1
      - id: pbm_2_low_energy_flag
        type: b1
      - id: pbm_2_error_i2c_mux
        type: b1
      - id: pbm_2_error_pca9534
        type: b1

  bit_mask_eps_pbm_3_error:
    seq:
      - id: reserve_2_bit
        type: b2
      - id: pbm_3_error_wreck_chrg_branch_2
        type: b1
      - id: pbm_3_error_wreck_chrg_branch_1
        type: b1
      - id: pbm_3_error_heat_2_temp_sensor_2
        type: b1
      - id: pbm_3_error_heat_2_temp_sensor_1
        type: b1
      - id: pbm_3_error_heat_2_heat
        type: b1
      - id: pbm_3_error_heat_2_ina238
        type: b1
      - id: pbm_3_error_heat_1_temp_sensor_2
        type: b1
      - id: pbm_3_error_heat_1_temp_sensor_1
        type: b1
      - id: pbm_3_error_heat_1_heat
        type: b1
      - id: pbm_3_error_heat_1_ina238
        type: b1
      - id: pbm_3_zero_energy_flag
        type: b1
      - id: pbm_3_low_energy_flag
        type: b1
      - id: pbm_3_error_i2c_mux
        type: b1
      - id: pbm_3_error_pca9534
        type: b1

  bit_mask_eps_pbm_4_error:
    seq:
      - id: reserve_2_bit
        type: b2
      - id: pbm_4_error_wreck_chrg_branch_2
        type: b1
      - id: pbm_4_error_wreck_chrg_branch_1
        type: b1
      - id: pbm_4_error_heat_2_temp_sensor_2
        type: b1
      - id: pbm_4_error_heat_2_temp_sensor_1
        type: b1
      - id: pbm_4_error_heat_2_heat
        type: b1
      - id: pbm_4_error_heat_2_ina238
        type: b1
      - id: pbm_4_error_heat_1_temp_sensor_2
        type: b1
      - id: pbm_4_error_heat_1_temp_sensor_1
        type: b1
      - id: pbm_4_error_heat_1_heat
        type: b1
      - id: pbm_4_error_heat_1_ina238
        type: b1
      - id: pbm_4_zero_energy_flag
        type: b1
      - id: pbm_4_low_energy_flag
        type: b1
      - id: pbm_4_error_i2c_mux
        type: b1
      - id: pbm_4_error_pca9534
        type: b1

  bit_mask_eps_pbm_1_controller_1_error:
    seq:
      - id: reserve_1_bit
        type: b1
      - id: pbm_1_controller_1_error_max17320
        type: b1
      - id: pbm_1_controller_1_error_charge
        type: b1
      - id: pbm_1_controller_1_error_dicharge
        type: b1
      - id: pbm_1_controller_1_chg_cntrl_flag
        type: b1
      - id: pbm_1_controller_1_dchg_cntrl_flag
        type: b1
      - id: pbm_1_controller_1_preq_chrg
        type: b1
      - id: pbm_1_controller_1_sc
        type: b1
      - id: pbm_1_controller_1_od
        type: b1
      - id: pbm_1_controller_1_oc
        type: b1
      - id: pbm_1_controller_1_ldet
        type: b1
      - id: pbm_1_controller_1_preqf
        type: b1
      - id: pbm_1_controller_1_odcp
        type: b1
      - id: pbm_1_controller_1_occp
        type: b1
      - id: pbm_1_controller_1_uvp
        type: b1
      - id: pbm_1_controller_1_ovp
        type: b1

  bit_mask_eps_pbm_1_controller_2_error:
    seq:
      - id: reserve_1_bit
        type: b1
      - id: pbm_1_controller_2_error_max17320
        type: b1
      - id: pbm_1_controller_2_error_charge
        type: b1
      - id: pbm_1_controller_2_error_dicharge
        type: b1
      - id: pbm_1_controller_2_chg_cntrl_flag
        type: b1
      - id: pbm_1_controller_2_dchg_cntrl_flag
        type: b1
      - id: pbm_1_controller_2_preq_chrg
        type: b1
      - id: pbm_1_controller_2_sc
        type: b1
      - id: pbm_1_controller_2_od
        type: b1
      - id: pbm_1_controller_2_oc
        type: b1
      - id: pbm_1_controller_2_ldet
        type: b1
      - id: pbm_1_controller_2_preqf
        type: b1
      - id: pbm_1_controller_2_odcp
        type: b1
      - id: pbm_1_controller_2_occp
        type: b1
      - id: pbm_1_controller_2_uvp
        type: b1
      - id: pbm_1_controller_2_ovp
        type: b1

  bit_mask_eps_pbm_2_controller_1_error:
    seq:
      - id: reserve_1_bit
        type: b1
      - id: pbm_2_controller_1_error_max17320
        type: b1
      - id: pbm_2_controller_1_error_charge
        type: b1
      - id: pbm_2_controller_1_error_dicharge
        type: b1
      - id: pbm_2_controller_1_chg_cntrl_flag
        type: b1
      - id: pbm_2_controller_1_dchg_cntrl_flag
        type: b1
      - id: pbm_2_controller_1_preq_chrg
        type: b1
      - id: pbm_2_controller_1_sc
        type: b1
      - id: pbm_2_controller_1_od
        type: b1
      - id: pbm_2_controller_1_oc
        type: b1
      - id: pbm_2_controller_1_ldet
        type: b1
      - id: pbm_2_controller_1_preqf
        type: b1
      - id: pbm_2_controller_1_odcp
        type: b1
      - id: pbm_2_controller_1_occp
        type: b1
      - id: pbm_2_controller_1_uvp
        type: b1
      - id: pbm_2_controller_1_ovp
        type: b1

  bit_mask_eps_pbm_2_controller_2_error:
    seq:
      - id: reserve_1_bit
        type: b1
      - id: pbm_2_controller_2_error_max17320
        type: b1
      - id: pbm_2_controller_2_error_charge
        type: b1
      - id: pbm_2_controller_2_error_dicharge
        type: b1
      - id: pbm_2_controller_2_chg_cntrl_flag
        type: b1
      - id: pbm_2_controller_2_dchg_cntrl_flag
        type: b1
      - id: pbm_2_controller_2_preq_chrg
        type: b1
      - id: pbm_2_controller_2_sc
        type: b1
      - id: pbm_2_controller_2_od
        type: b1
      - id: pbm_2_controller_2_oc
        type: b1
      - id: pbm_2_controller_2_ldet
        type: b1
      - id: pbm_2_controller_2_preqf
        type: b1
      - id: pbm_2_controller_2_odcp
        type: b1
      - id: pbm_2_controller_2_occp
        type: b1
      - id: pbm_2_controller_2_uvp
        type: b1
      - id: pbm_2_controller_2_ovp
        type: b1

  bit_mask_eps_pbm_3_controller_1_error:
    seq:
      - id: reserve_1_bit
        type: b1
      - id: pbm_3_controller_1_error_max17320
        type: b1
      - id: pbm_3_controller_1_error_charge
        type: b1
      - id: pbm_3_controller_1_error_dicharge
        type: b1
      - id: pbm_3_controller_1_chg_cntrl_flag
        type: b1
      - id: pbm_3_controller_1_dchg_cntrl_flag
        type: b1
      - id: pbm_3_controller_1_preq_chrg
        type: b1
      - id: pbm_3_controller_1_sc
        type: b1
      - id: pbm_3_controller_1_od
        type: b1
      - id: pbm_3_controller_1_oc
        type: b1
      - id: pbm_3_controller_1_ldet
        type: b1
      - id: pbm_3_controller_1_preqf
        type: b1
      - id: pbm_3_controller_1_odcp
        type: b1
      - id: pbm_3_controller_1_occp
        type: b1
      - id: pbm_3_controller_1_uvp
        type: b1
      - id: pbm_3_controller_1_ovp
        type: b1

  bit_mask_eps_pbm_3_controller_2_error:
    seq:
      - id: reserve_1_bit
        type: b1
      - id: pbm_3_controller_2_error_max17320
        type: b1
      - id: pbm_3_controller_2_error_charge
        type: b1
      - id: pbm_3_controller_2_error_dicharge
        type: b1
      - id: pbm_3_controller_2_chg_cntrl_flag
        type: b1
      - id: pbm_3_controller_2_dchg_cntrl_flag
        type: b1
      - id: pbm_3_controller_2_preq_chrg
        type: b1
      - id: pbm_3_controller_2_sc
        type: b1
      - id: pbm_3_controller_2_od
        type: b1
      - id: pbm_3_controller_2_oc
        type: b1
      - id: pbm_3_controller_2_ldet
        type: b1
      - id: pbm_3_controller_2_preqf
        type: b1
      - id: pbm_3_controller_2_odcp
        type: b1
      - id: pbm_3_controller_2_occp
        type: b1
      - id: pbm_3_controller_2_uvp
        type: b1
      - id: pbm_3_controller_2_ovp
        type: b1

  bit_mask_eps_pbm_4_controller_1_error:
    seq:
      - id: reserve_1_bit
        type: b1
      - id: pbm_4_controller_1_error_max17320
        type: b1
      - id: pbm_4_controller_1_error_charge
        type: b1
      - id: pbm_4_controller_1_error_dicharge
        type: b1
      - id: pbm_4_controller_1_chg_cntrl_flag
        type: b1
      - id: pbm_4_controller_1_dchg_cntrl_flag
        type: b1
      - id: pbm_4_controller_1_preq_chrg
        type: b1
      - id: pbm_4_controller_1_sc
        type: b1
      - id: pbm_4_controller_1_od
        type: b1
      - id: pbm_4_controller_1_oc
        type: b1
      - id: pbm_4_controller_1_ldet
        type: b1
      - id: pbm_4_controller_1_preqf
        type: b1
      - id: pbm_4_controller_1_odcp
        type: b1
      - id: pbm_4_controller_1_occp
        type: b1
      - id: pbm_1_controller_1_uvp
        type: b1
      - id: pbm_4_controller_1_ovp
        type: b1

  bit_mask_eps_pbm_4_controller_2_error:
    seq:
      - id: reserve_1_bit
        type: b1
      - id: pbm_4_controller_2_error_max17320
        type: b1
      - id: pbm_4_controller_2_error_charge
        type: b1
      - id: pbm_4_controller_2_error_dicharge
        type: b1
      - id: pbm_4_controller_2_chg_cntrl_flag
        type: b1
      - id: pbm_4_controller_2_dchg_cntrl_flag
        type: b1
      - id: pbm_4_controller_2_preq_chrg
        type: b1
      - id: pbm_4_controller_2_sc
        type: b1
      - id: pbm_4_controller_2_od
        type: b1
      - id: pbm_4_controller_2_oc
        type: b1
      - id: pbm_4_controller_2_ldet
        type: b1
      - id: pbm_4_controller_2_preqf
        type: b1
      - id: pbm_4_controller_2_odcp
        type: b1
      - id: pbm_4_controller_2_occp
        type: b1
      - id: pbm_4_controller_2_uvp
        type: b1
      - id: pbm_4_controller_2_ovp
        type: b1

  bit_mask_eps_pbm_beacon_error_1:
    seq:
      - id: pbm_2_controller_1_error_charge
        type: b1
      - id: pbm_2_controller_1_error_dicharge
        type: b1
      - id: pbm_1_controller_2_error_max17320
        type: b1
      - id: pbm_1_controller_2_error_charge
        type: b1
      - id: pbm_1_controller_2_error_dicharge
        type: b1
      - id: pbm_1_controller_1_error_max17320
        type: b1
      - id: pbm_1_controller_1_error_charge
        type: b1
      - id: pbm_1_controller_1_error_dicharge
        type: b1

  bit_mask_eps_pbm_beacon_error_2:
    seq:
      - id: pbm_3_controller_2_error_dicharge
        type: b1
      - id: pbm_3_controller_1_error_max17320
        type: b1
      - id: pbm_3_controller_1_error_charge
        type: b1
      - id: pbm_3_controller_1_error_dicharge
        type: b1
      - id: pbm_2_controller_2_error_max17320
        type: b1
      - id: pbm_2_controller_2_error_charge
        type: b1
      - id: pbm_2_controller_2_error_dicharge
        type: b1
      - id: pbm_2_controller_1_error_max17320
        type: b1

  bit_mask_eps_pbm_beacon_error_3:
    seq:
      - id: pbm_4_controller_2_error_max17320
        type: b1
      - id: pbm_4_controller_2_error_charge
        type: b1
      - id: pbm_4_controller_2_error_dicharge
        type: b1
      - id: pbm_4_controller_1_error_max17320
        type: b1
      - id: pbm_4_controller_1_error_charge
        type: b1
      - id: pbm_4_controller_1_error_dicharge
        type: b1
      - id: pbm_3_controller_2_error_max17320
        type: b1
      - id: pbm_3_controller_2_error_charge
        type: b1
 
  bit_mask_eps_beacon_error_1:
    seq:
      - id: pmm_error_pwr_ch_state_pbms_logic
        type: b1
      - id: pmm_error_pwr_ch_state_vbat_2_ef_2
        type: b1
      - id: pmm_error_pwr_ch_state_vbat_2_ef_1
        type: b1
      - id: pmm_error_pwr_ch_state_vbat_1_ef_2
        type: b1
      - id: pmm_error_pwr_ch_state_vbat_1_ef_1
        type: b1
      - id: pmm_error_pwr_ch_state_can_backup
        type: b1
      - id: pmm_error_pwr_ch_state_can_main
        type: b1
      - id: pmm_error_i2c_gpio_1
        type: b1
 
  bit_mask_eps_beacon_error_2:
    seq:
      - id: pam_error_i2c_mux_2
        type: b1
      - id: pam_error_i2c_mux_1
        type: b1
      - id: pam_error_i2c_gpio
        type: b1
      - id: pam_error_state_ldo
        type: b1
      - id: pam_error_state_dc_dc
        type: b1
      - id: pdm_error_i2c_mux
        type: b1
      - id: pdm_error_i2c_gpio_2
        type: b1
      - id: pdm_error_i2c_gpio_1
        type: b1

  vector:
    seq:
      - id: x
        type: s2
      - id: y
        type: s2
      - id: z
        type: s2

  ip_address:
    seq:
      - id: arr
        type: u1
        repeat: expr
        repeat-expr: 4
    instances:
      str:
        value: arr[0].to_s + "." + arr[1].to_s + "." + arr[2].to_s + "." + arr[3].to_s