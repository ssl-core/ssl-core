export module decision.evaluators:ievaluator;

export namespace decision {

class IEvaluator {
 public:
  IEvaluator() = default;

  IEvaluator(const IEvaluator&) = delete;
  IEvaluator& operator=(const IEvaluator&) = delete;

  IEvaluator(IEvaluator&&) = default;
  IEvaluator& operator=(IEvaluator&&) = default;

  virtual ~IEvaluator() = default;

  virtual void run() = 0;
  virtual void reset() = 0;
};

} // namespace decision
