const context: Record<string, any> = {};

export const provide = (key: string, value: any) => {
  context[key] = value;
};

export const inject = <T>(key: string): T => {
  return context[key] as T;
};
