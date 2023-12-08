use std::io::{BufRead, BufReader, Read, Write};
use std::sync::Arc;
use std::time::Duration;
use serialport::{DataBits, FlowControl, Parity, SerialPort, SerialPortBuilder, SerialPortInfo, StopBits};
use serialport::DataBits::Eight;
use tokio::sync::{Mutex, MutexGuard};
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
pub struct ServerModel {
    serial_port_name: Arc<Mutex<String>>,
    serial_port_io: Arc<Mutex<Option<SerialPortIO>>>,
}

impl ServerModel {
    pub fn new() -> ServerModel {
        ServerModel {
            serial_port_name: Arc::new(Mutex::new("".to_string())),
            serial_port_io: Arc::new(Mutex::new(None)),
        }
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

    pub async fn get_co2(&self) -> Option<u32> {
        return None;
    }

    pub async fn get_temperature(&self) -> u32 {
        return 33;
    }

    pub async fn get_light_intensity(&self) -> u32 {
        return 33;
    }

    pub async fn get_humidity(&self) -> u32 {
        return 33;
    }
}