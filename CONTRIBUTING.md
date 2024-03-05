# Contributing to the IPAddress Gem

[![Build Status](https://github.com/ipaddress-gem/ipaddress/actions/workflows/tests/badge.svg)](https://github.com/ipaddress-gem/ipaddress/actions)
[![Code Climate](https://codeclimate.com/github/ipaddress-gem/ipaddress/badges/gpa.svg)](https://codeclimate.com/github/ipaddress-gem/ipaddress)
[![Dependency Status](https://www.versioneye.com/user/projects/57001305fcd19a0051853bde/badge.svg?style=flat)](https://www.versioneye.com/user/projects/57001305fcd19a0051853bde)

This gem is run by people who have jobs.  So please understand if we can't always prioritize PRs and issues.

You can help by making your code submissions.  We can't promise a specific turnaround time, or that your code will be incorporated but all submissions are appreciated.

## Steps to Submit a Pull Request

* [Fork](https://help.github.com/articles/fork-a-repo) the project on GitHub.
* Make your feature addition or bug fix [in a feature branch](https://github.com/Kunena/Kunena-Forum/wiki/Create-a-new-branch-with-git-and-manage-branches). (Include a description of your changes in the PR)
* Push your feature branch to GitHub.
* Send a [Pull Request](https://help.github.com/articles/using-pull-requests)

## Style Guide

We will require that you adhere to the [ruby-style-guide](https://github.com/bbatsov/ruby-style-guide) for your code submissions.

## Testing

To run the tests you need to have:

1. The development dependencies installed with `bin/setup`.
2. Then just run the command `bin/rake test`.

## Test Coverage

All submissions of code must include test coverage which describes intent and expected behavior.
The test suite used by this gem is [Minitest](https://github.com/seattlerb/minitest)

Unit tests are expected to execute quickly.  We will ask you to revise any long-running tests.

## Versioning

This repo uses [semantic versioning](http://semver.org/).

Before release, update version in `lib/ipaddress/version.rb` file.
