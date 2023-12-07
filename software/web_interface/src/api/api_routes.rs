use axum::{routing::{get, post}, http::{StatusCode, Uri}, Router, Json};
use axum::extract::State;
use serde::{Deserialize, Serialize};
use axum::response::{Html, Redirect};
use serialport::SerialPortInfo;

use crate::api::errors::ApiErrorCode;
use crate::api::response::ApiResponse;
use crate::api::response::ApiResponse::{ErrApiResponse, OkApiResponse};
use crate::api::custom_extractors::ApiJson;
use crate::api::errors::ApiErrorCode::PortNotFound;
use crate::api::file_server::serve_html;
use crate::model::model::ServerModel;

#[derive(Serialize)]
struct EnvironmentVariables {
    co2: u32,
    humidity: u32,
    luminance: u32,
    temperature: u32,
}

#[derive(Serialize)]
struct SerialPortList {
    ports: Vec<String>,
}

#[derive(Deserialize)]
struct SerialPort {
    port: String,
}

#[derive(Deserialize, Debug)]
struct LightConfig {
    r: u8,
    g: u8,
    b: u8,
    p: u8,
}

#[derive(Deserialize, Debug)]
struct FanConfig {
    speed: u8,
}

pub fn routes(model: ServerModel) -> Router {
    Router::new()
        .route("/", get(get_root_handler))
        .route("/dashboard", get(get_dashboard_handler))
        .route("/light_config", get(get_light_config_handler))
        .route("/light_config", post(post_light_config_handler))
        .route("/fan_config", get(get_fan_config_handler))
        .route("/fan_config", post(post_fan_config_handler))
        .route("/serial_ports", get(get_serial_ports_handler))
        .route("/serial_ports", post(post_serial_ports_handler))
        .fallback(fallback_handler)
        .with_state(model)
}

async fn get_root_handler() -> Redirect {
    Redirect::permanent("/dashboard")
}

async fn get_dashboard_handler() -> ApiResponse<Html<String>> {
    serve_html("assets/web/dashboard.html")
}

async fn get_light_config_handler() -> ApiResponse<Html<String>> {
    serve_html("assets/web/light_config.html")
}

async fn get_fan_config_handler() -> ApiResponse<Html<String>> {
    serve_html("assets/web/fan_config.html")
}

async fn post_light_config_handler(State(server_model): State<ServerModel>, ApiJson(light_config): ApiJson<LightConfig>) -> ApiResponse<&'static str> {
    println!("Light config updated to: {:?}", light_config);
    server_model.update_light_config(light_config.r, light_config.g, light_config.b, light_config.p).await;
    OkApiResponse(StatusCode::OK, "Light config updated")
}

async fn post_fan_config_handler(State(server_model): State<ServerModel>, ApiJson(fan_config): ApiJson<FanConfig>) -> ApiResponse<&'static str> {
    println!("Fan config updated to: {:?}", fan_config);
    server_model.update_fan_config(fan_config.speed).await;
    OkApiResponse(StatusCode::OK, "Fan config updated")
}

async fn get_serial_ports_handler() -> ApiResponse<Json<SerialPortList>> {
    let ports = serialport::available_ports().unwrap_or(Vec::new());
    let port_names: Vec<String> = ports.into_iter().map(|SerialPortInfo {port_name, .. }| port_name).collect();

    OkApiResponse(StatusCode::OK, Json(SerialPortList { ports: port_names }))
}

async fn post_serial_ports_handler(State(server_model): State<ServerModel>, ApiJson(serial_port_name): ApiJson<SerialPort>) -> ApiResponse<String> {
    match server_model.set_port(serial_port_name.port).await {
        Ok(_) => OkApiResponse(StatusCode::OK, "Port updated".to_string()),
        Err(_) => ErrApiResponse(PortNotFound, "Port not found".to_string()),
    }
}

async fn fallback_handler(uri: Uri) -> ApiResponse<()> {
    ErrApiResponse(ApiErrorCode::UriNotFound, format!("Uri \'{}\' not found", uri))
}
