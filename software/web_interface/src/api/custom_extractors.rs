use axum_macros::{FromRequest, FromRequestParts};
use axum::extract::rejection::{JsonRejection, PathRejection};

use crate::api::errors::ApiErrorCode;
use crate::api::response::ApiResponse;
use crate::api::response::ApiResponse::ErrApiResponse;

#[derive(FromRequest)]
#[from_request(via(axum::extract::Json), rejection(ApiResponse<()>))]
pub struct ApiJson<T> (pub T);

impl From<JsonRejection> for ApiResponse<()> {
    fn from(_: JsonRejection) -> Self {
        ErrApiResponse(ApiErrorCode::InvalidRequestBody, "Invalid request".to_string())
    }
}

#[derive(FromRequestParts)]
#[from_request(via(axum::extract::Path), rejection(ApiResponse<()>))]
pub struct ApiPath<T> (pub T);

impl From<PathRejection> for ApiResponse<()> {
    fn from(_: PathRejection) -> Self {
        ErrApiResponse(ApiErrorCode::InvalidRequestParameter, "Invalid Request".to_string())
    }
}