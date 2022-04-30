#' Log UDP broadcast packets to a file
#'
#' Listen for broadcast payloads on `port` and log them directly to `file`.
#' The listener will continue forever or until a `CTRL-C` (`SIGINT`) is encountered.
#'
#' @note The current implementation of the logger assumes character payload data.
#' Please file an issue if you need it to handle raw vectors.
#'
#' @param port UDP broadcast port to listen on
#' @param path path (including filename) to write records to
#' @return none
#' @export
#' @examples \dontrun{
#' udp_file_logger(22222L, "~/Data/davis.json")
#' }
udp_file_logger <- function(port, path) {

  port <- as.integer(port[1])
  path <- path.expand(path[1])

  stopifnot("Logfile directory does not exist"=dir.exists(dirname(path)))

  invisible(.Call(
    "_udpbroadcastreceiver_udp_path_logger",
    port,
    path,
    PACKAGE = "udpbroadcastreceiver"
  ))

}

#' Log UDP broadcast packets to a file
#'
#' Listen for broadcast payloads on `port` and call `callback` with the payload data.
#' The listener will continue forever or until a `CTRL-C` (`SIGINT`) is encountered.
#'
#' The `callback` function must take one parameter which contains the payload data.
#' The callback return value should be `TRUE` if the listener should be stopped and
#' `FALSE` if it should continue listening for broadcast payloads and calling the
#' `callback` function.
#'
#' @note The current implementation of the logger assumes character payload data.
#' Please file an issue if you need it to handle raw vectors.
#'
#' @param port UDP broadcast port to listen on
#' @param callback R callback function (see Details)
#' @return none
#' @export
#' @examples \dontrun{
#' udp_callback_logger(22222L, \(x) { writeLines(x); return(FALSE) })
#' }
udp_callback_logger <- function(port, callback) {

  port <- as.integer(port[1])

  stopifnot("The 'callback' parameter must be a function." = is.function(callback))
  stopifnot("The 'callback' function takes one parameter." = length(formals(callback)) == 1)

  invisible(.Call(
    "_udpbroadcastreceiver_udp_callback_logger",
    port,
    callback,
    PACKAGE = "udpbroadcastreceiver"
  ))

}
