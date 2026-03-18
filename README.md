# ZeroTrust TradeCore

An enterprise-grade, high-performance C++ trading engine paired with a robust zero-trust CI/CD pipeline for automated, secure, and resilient deployments.

## Overview

The trading engine features a highly optimized Order Book implemented in C++. It utilizes a custom `FastBitmap` 4-level hierarchical data structure to achieve $O(1)$ limit price lookups using hardware-accelerated bit manipulation functions (`__builtin_clzll`, `__builtin_ctzll`). It is designed for maximum throughput and minimal latency.

The surrounding infrastructure includes a complete "Zero Trust" pipeline that prioritizes security, repeatability, and zero-downtime upgrades:

- **Continuous Integration:** GitHub Actions with CodeQL for comprehensive Static Application Security Testing (SAST).
- **Containerization:** Minimal Docker profiles using multi-stage builds and aggressive compiler optimizations (`-O3`, `-march=native`, `-flto`) securely run as an unprivileged user.
- **Infrastructure as Code (IaC):** Automated Kubernetes cluster provisioning and environment configuration using Terraform.
- **Continuous Deployment:** Kubernetes manifests engineered for Blue/Green deployment strategies, ensuring seamless traffic cutover and easy rollbacks for the trading engine.

## Getting Started

### Prerequisites

- Docker
- Kubernetes environment
- Terraform

### Building the Engine

You can build the engine as a docker container directly:

```bash
docker build -t trading-engine:latest .
```

### Running Tests

The engine includes a suite of unit tests for the order matching logic:

```bash
./engine --test
```

### Infrastructure Deployment

1. Provision the cluster:

```bash
cd terraform
terraform init
terraform apply
```

2. Apply the Kubernetes manifests for the Blue/Green deployments:

```bash
kubectl apply -f k8s/blue.yaml
kubectl apply -f k8s/service.yaml
```
