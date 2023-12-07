use axum::{
    response::{IntoResponse, Response},
    http::StatusCode,
    Json,
};
use serde::Serialize;
use crate::api::errors::ApiErrorCode;

pub enum ApiResponse<T: IntoResponse> {
    OkApiResponse(StatusCode, T),
    ErrApiResponse(ApiErrorCode, String),
}

impl<T: IntoResponse> IntoResponse for ApiResponse<T> {
    fn into_response(self) -> Response {
        match self {
            ApiResponse::OkApiResponse(http_code, data) =>
                (http_code, data).into_response(),
            ApiResponse::ErrApiResponse(api_error_code, message) =>
                (api_error_code.http_status_code(), message).into_response(),
        }
    }
}