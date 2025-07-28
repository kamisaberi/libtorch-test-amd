#include <torch/torch.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>
int main() {

	const int num_threads = std::thread::hardware_concurrency();  // Use all available cores
	torch::set_num_threads(num_threads);
	std::cout << "Using " << torch::get_num_threads() << " threads for LibTorch" << std::endl;


    torch::Device device = torch::kCPU;
    if (torch::cuda::is_available()) {
        device = torch::kCUDA;
        std::cout << "Using CUDA device" << std::endl;
    } else {
        std::cout << "Using CPU device" << std::endl;
    }
    torch::TensorOptions tensor_options = torch::TensorOptions().dtype(torch::kFloat32).device(device);
    torch::Tensor x = torch::linspace(-M_PI, M_PI, 2000, tensor_options);
    torch::Tensor y = torch::sin(x);
    auto p_opts = torch::TensorOptions().dtype(torch::kFloat32).device(device); // p can also be on the same device
    torch::Tensor p = torch::tensor({1, 2, 3}, p_opts);
    torch::Tensor xx = x.unsqueeze(-1).pow(p);
    torch::nn::Sequential model(
    torch::nn::Linear(3, 1),
    torch::nn::Flatten(torch::nn::FlattenOptions().start_dim(0).end_dim(1))
    );
    model->to(device);
    auto loss_fn = torch::nn::MSELoss(torch::nn::MSELossOptions().reduction(torch::kSum));
    float learning_rate = 1e-6;
	auto start_time = std::chrono::steady_clock::now();
    for (int t = 0; t < 200000; ++t) {
        torch::Tensor y_pred = model->forward(xx);
        torch::Tensor loss = loss_fn(y_pred, y);
        if (t % 10000 == 9999) {
            std::cout << t << " " << loss.item<float>() << std::endl;
        }
        model->zero_grad();
        loss.backward();
        torch::NoGradGuard no_grad;
        for (auto& param : model->parameters()) {
            if (param.grad().defined()) {
                param.sub_(learning_rate * param.grad());
            }
        }
    }



	 auto end_time = std::chrono::steady_clock::now();
	 auto duration = end_time - start_time;
	 auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	 std::cout << "Total loop duration: " << duration_ms.count() << " milliseconds." << std::endl;
	 for (auto& param : model->parameters()) {
        if (param.grad().defined()) {
            std::cout << param << std::endl;
        }
    }
    // auto linear_layer = model->ptr<torch::nn::LinearImpl>(0);
    // auto bias = linear_layer->bias.item<float>();
    // auto w1 = linear_layer->weight.index({0, 0}).item<float>();
    // auto w2 = linear_layer->weight.index({0, 1}).item<float>();
    // auto w3 = linear_layer->weight.index({0, 2}).item<float>();
    // std::cout << std::fixed << std::setprecision(5);
    // std::cout << "Result: y = " << bias << " + " << w1 << " x + " << w2 << " x^2 + " << w3 << " x^3" << std::endl;
    return 0;
}
