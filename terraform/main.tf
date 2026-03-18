terraform {
  required_providers {
    kind = {
      source  = "tehcyx/kind"
      version = "~> 0.2.0"
    }
  }
}

provider "kind" {}

resource "kind_cluster" "default" {
  name           = "zero-trust-cluster"
  wait_for_ready = true
}