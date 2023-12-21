use axum::http::StatusCode;

pub enum ApiErrorCode {
    InvalidRequestBody,
    InvalidRequestParameter,
    PageSourceMissing,
    PortNotFound,
    UriNotFound,
}

impl ApiErrorCode {
    pub fn http_status_code(&self) -> StatusCode {
        match self {
            ApiErrorCode::InvalidRequestBody => StatusCode::BAD_REQUEST,
            ApiErrorCode::InvalidRequestParameter => StatusCode::BAD_REQUEST,
            ApiErrorCode::UriNotFound => StatusCode::NOT_FOUND,
            ApiErrorCode::PageSourceMissing => StatusCode::INTERNAL_SERVER_ERROR,
            ApiErrorCode::PortNotFound => StatusCode::NOT_FOUND,
        }
    }
}