use serialport::SerialPort;
use crate::api::api_routes;
use crate::model::model::ServerModel;

mod api;
mod model;

#[tokio::main]
async fn main() {
    let server_model: ServerModel = ServerModel::new();
    let app = api_routes::routes(server_model);

    let listener = tokio::net::TcpListener::bind("localhost:3000").await.unwrap();
    axum::serve(listener, app).await.unwrap();
}
