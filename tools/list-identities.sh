#!/bin/bash
security find-identity -v -p codesigning -p macappstore | grep "Developer ID"
