#' Receive and Log or Process UDP Broadcast Payloads
#'
#' Many remote sensors, such as weather stations, have the ability to broadcast
#' their record payloads over UDP. Tools are provided to listen for these broadcast packets
#' and either log them directly to a file or process them with callback functions.
#'
#' @md
#' @name udpbroadcastreceiver
#' @keywords internal
#' @author Bob Rudis (bob@@rud.is)
#' @importFrom Rcpp sourceCpp
#' @useDynLib udpbroadcastreceiver, .registration = TRUE
"_PACKAGE"
