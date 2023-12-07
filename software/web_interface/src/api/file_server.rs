use std::fs;
use axum::http::StatusCode;
use axum::response::Html;
use crate::api::errors::ApiErrorCode::PageSourceMissing;
use crate::api::response::ApiResponse;
use crate::api::response::ApiResponse::{ErrApiResponse, OkApiResponse};

pub fn serve_html(path: impl Into<String> ) -> ApiResponse<Html<String>> {
    match fs::read_to_string(path.into()) {
        Ok(html_content) => OkApiResponse(StatusCode::OK, Html(html_content)),
        Err(_) => ErrApiResponse(PageSourceMissing, "Page source missing".to_string()),
    }
}