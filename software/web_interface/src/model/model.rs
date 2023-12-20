use std::io::{BufRead, BufReader, Read, Write};
use std::sync::Arc;
use std::thread;
use std::time::Duration;
use serialport::{DataBits, FlowControl, Parity, SerialPort, SerialPortBuilder, SerialPortInfo, StopBits};
use serialport::DataBits::Eight;
use tokio::sync::{Mutex, MutexGuard};
use tokio::task;
use tokio::time::Timeout;

pub struct SerialPortIO {
    serial_port: Box<dyn SerialPort>,
    serial_port_reader: BufReader<Box<dyn SerialPort>>,
}

impl SerialPortIO {
    pub fn new(serial_port: Box<dyn SerialPort>) -> SerialPortIO {
        SerialPortIO {
            serial_port_reader: BufReader::new(serial_port.try_clone().expect("Serial port clone error")),
            serial_port,
        }
    }
}

#[derive(Clone)]
pub struct SensorData {
    pub light: Option<u16>,
    pub co2: Option<u16>,
    pub temperature: Option<u16>,
    pub humidity: Option<u16>,
    pub sound: Option<u16>,
}

impl SensorData {
    pub fn new() -> SensorData {
        SensorData {
            light: None,
            co2: None,
            temperature: None,
            humidity: None,
            sound: None,
        }
    }
}

#[derive(Clone)]
pub struct ServerModel {
    serial_port_name: Arc<Mutex<String>>,
    serial_port_io: Arc<Mutex<Option<SerialPortIO>>>,
    sensor_data: Arc<Mutex<SensorData>>
}

impl ServerModel {
    pub fn new() -> ServerModel {
        let model = ServerModel {
            serial_port_name: Arc::new(Mutex::new("".to_string())),
            serial_port_io: Arc::new(Mutex::new(None)),
            sensor_data: Arc::new(Mutex::new(SensorData::new()))
        };

        model
    }

    pub async fn has_port(&self) -> bool {
        self.serial_port_io.lock().await.is_some()
    }

    pub async fn set_port(&self, port_name: String) -> Result<(), ()> {
        let mut serial_port_io_guard = self.serial_port_io.lock().await;
        let mut serial_port_name_guard = self.serial_port_name.lock().await;

        println!("IN");

        if *serial_port_name_guard == port_name {
            println!("Already open: {}", port_name);
            return Ok(());
        }

        let serial_port_builder = serialport::new(&port_name, 9600)
            .data_bits(DataBits::Eight)
            .flow_control(FlowControl::None)
            .parity(Parity::None)
            .stop_bits(StopBits::One)
            .timeout(Duration::from_millis(1000));

        match serial_port_builder.open() {
            Ok(serial_port) => {
                *serial_port_name_guard = port_name;
                *serial_port_io_guard = Some(SerialPortIO::new(serial_port));
                println!("Open: {}", serial_port_name_guard);
                Ok(())
            },
            Err(error) => {
                println!("Error: {}", error);
                Err(())
            },
        }
    }

    pub async fn update_light_config(&self, r: u8, g: u8, b: u8, p: u8) {
        if let Some(serial_port_io) = &mut *self.serial_port_io.lock().await {
            println!("Writing led config...");
            let message = [0u8, p, r, g, b];
            let _ = serial_port_io.serial_port.write(&message);

            let mut buffer = String::new();
            let _ = serial_port_io.serial_port_reader.read_line(&mut buffer);
            println!("{}", buffer);
        }
    }

    pub async fn update_fan_config(&self, speed: u8) {
        if let Some(serial_port_io) = &mut *self.serial_port_io.lock().await {
            println!("Writing fan config...");
            let message = [1u8, speed];
            let _ = serial_port_io.serial_port.write(&message);

            let mut buffer = String::new();
            let _ = serial_port_io.serial_port_reader.read_line(&mut buffer);
            println!("{}", buffer);
        }
    }

    pub async fn update_sensor_data(&self, serial_reader: &mut impl BufRead) -> SensorData {
        let mut first_byte: u8 = 0;
        let mut sensors_data = self.sensor_data.lock().await;

        while serial_reader.read_exact(std::slice::from_mut(&mut first_byte)) {
            match first_byte {
                0 => { // CO2
                    sensors_data.co2 = Self::read_co2(serial_reader);
                }
                1 => { // Humidity
                    sensors_data.humidity = Self::read_humidity(serial_reader);
                }
                2 => { // Temperature
                    sensors_data.temperature = Self::read_temperature(serial_reader);
                }
                3 => { // Sound
                    sensors_data.sound = Self::read_sound(serial_reader);
                }
                4 => { // Light
                    sensors_data.light = Self::read_light(serial_reader);
                }
                _ => {}
            }
        }

        sensors_data.clone()
    }

    fn read_co2(serial_reader: &mut impl BufRead) -> Option<u16> {
        let mut input_buff =[0u8; 2];

        if let Ok(()) = serial_reader.read_exact(&mut input_buff) {
            Some(u16::from_be_bytes(input_buff))
        } else {
            None
        }
    }

    fn read_temperature(serial_reader: &mut impl BufRead) -> Option<u16> {
        let mut input_buff =[0u8; 2];

        if let Ok(()) = serial_reader.read_exact(&mut input_buff) {
            Some(u16::from_be_bytes(input_buff))
        } else {
            None
        }
    }

    fn read_sound(serial_reader: &mut impl BufRead) -> Option<u16> {
        let mut input_buff = [0u8; 2];

        if let Ok(()) = serial_reader.read_exact(&mut input_buff) {
            Some(u16::from_be_bytes(input_buff))
        } else {
            None
        }
    }

    fn read_humidity(serial_reader: &mut impl BufRead) -> Option<u16> {
        let mut input_buff = [0u8; 2];

        if let Ok(()) = serial_reader.read_exact(&mut input_buff) {
            Some(u16::from_be_bytes(input_buff))
        } else {
            None
        }
    }

    fn read_light(serial_reader: &mut impl BufRead) -> Option<u16> {
        let mut input_buff = [0u8; 2];

        if let Ok(()) = serial_reader.read_exact(&mut input_buff) {
            Some(u16::from_be_bytes(input_buff))
        } else {
            None
        }
    }

    pub async fn get_sensors_data(&self) -> SensorData {
        println!("ASDFASDF");
        self.update_sensor_data(&mut self.serial_port_io.lock().await.as_mut().unwrap().serial_port_reader).await
    }
}