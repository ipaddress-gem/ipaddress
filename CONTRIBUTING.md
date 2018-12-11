# Contributing to the IPAddress Gem

[![Build Status](https://travis-ci.org/ipaddress-gem/ipaddress.svg?branch=master)](https://travis-ci.org/ipaddress-gem/ipaddress) [![Code Climate](https://codeclimate.com/github/ipaddress-gem/ipaddress/badges/gpa.svg)](https://codeclimate.com/github/ipaddress-gem/ipaddress) [![Dependency Status](https://www.versioneye.com/user/projects/57001305fcd19a0051853bde/badge.svg?style=flat)](https://www.versioneye.com/user/projects/57001305fcd19a0051853bde)

This gem is run by people who have jobs.  So please understand if we can't always prioritize PRs and issues.

You can help by making your code submissions.  We can't promise a specific turnaround time, or that your code will be incorporated but all submissions are appreciated.

## Steps to Submit a Pull Request

* [Fork](https://help.github.com/articles/fork-a-repo) the project on GitHub.
* Make your feature addition or bug fix [in a feature branch](https://github.com/Kunena/Kunena-Forum/wiki/Create-a-new-branch-with-git-and-manage-branches). (Include a description of your changes in the PR)
* Push your feature branch to GitHub.
* Send a [Pull Request](https://help.github.com/articles/using-pull-requests)

## Style Guide

We will require that you adhere to the [ruby-style-guide](https://github.com/bbatsov/ruby-style-guide) for your code submissions.

## Test Coverage

All submissions of code must include test coverage which describes intent and expected behavior.  The test suite used by this gem is [Minitest](https://github.com/seattlerb/minitest)

Unit tests are expected to execute quickly.  We will ask you to revise any long-running tests.

We intend to add [Travis CI](https://travis-ci.org/) for automatic execution of branch tests.

## Versioning: Jeweler and Semantic Versioning

This repo uses [semantic versioning](http://semver.org/) implemented by the [Jeweler Gem](https://github.com/technicalpickles/jeweler).  Please do not update the version by editting files, instead, you may increment or modify the version using the Rake tasks added by Jeweler.

```
$ rake -T | grep version
rake version             # Displays the current version
rake version:bump:major  # Bump the major version by 1
rake version:bump:minor  # Bump the a minor version by 1
rake version:bump:patch  # Bump the patch version by 1
rake version:write       # Writes out an explicit version
```