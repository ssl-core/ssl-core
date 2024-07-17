export const formatAvatar = (name: string) => {
  if (!name) {
    return "";
  }

  const parts = name.trim().split(" ");
  let initials = "";

  if (parts.length > 1) {
    initials = parts[0].charAt(0) + parts[1].charAt(0);
  } else {
    initials = parts[0].substring(0, 2);
  }

  return initials.toUpperCase();
};

export const formatTime = (time: Date) => {
  const minutes = time.getMinutes().toString().padStart(2, "0");
  const seconds = time.getSeconds().toString().padStart(2, "0");
  return `${minutes}:${seconds}`;
};

export const formatScore = (home: number, away: number) => {
  return `${home}:${away}`;
};

export const formatGameStage = (stage: string) => {
  return stage
    .replace(/_/g, " ")
    .replace("GAME STAGE", "")
    .replace("NORMAL", "")
    .replace("FIRST", "1ST")
    .replace("SECOND", "2ND")
    .toLowerCase();
};
