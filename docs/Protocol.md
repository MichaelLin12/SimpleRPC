# Message Format

The message format will vary slightly based on the message type. Below we list the various kinds of message formats.

- The following message format will be used for Msg::Send

`| Message Type | Message size | Function name | Args |`

- The following message format will be used for Msg::Resp

`| Message Type | Message Size | Return Value |`

- The following message format will be used for Msg::Void

`| Message Type |`

- The following message format will be used for Msg::Err

`| Message Type | Error Message |`