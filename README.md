hyper-serve
===========

A server for serving whole or portions of hypertext (Ted Nelson style) inspired files.

Currently 3 PULL request types are supported. Full file, partial by ancor, and partial by range.

Full file request:
==================

 ./client PULL URL/path/to/file

Ancor request:
==============

 ./client PULL URL/path/to/file#ancor

Where #ancor is an ancor point defined in the file requested.

Range request:
==============

 ./client PULL URL/path/to/file@Start,End

Start and End must be integer values, or ^ and $ denoting the start and end of the file respectively.
