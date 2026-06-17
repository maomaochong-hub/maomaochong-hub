<!-- 1. 波浪图片（Capsule Render 生成） -->
<img src="https://capsule-render.vercel.app/api?type=waving&color=0:009999,1:006666&height=180&section=header&text=%E4%BA%BA%E7%9A%86%E7%9F%A5%E6%9C%89%E7%94%A8%E4%B9%8B%E7%94%A8%2C%20%E8%80%8C%E8%8E%AB%E7%9F%A5%E6%97%A0%E7%94%A8%E4%B9%8B%E7%94%A8%E4%B9%9F&fontSize=20&fontAlignY=35&animation=fadeIn" alt="wave banner" />

<!-- 2. 定义样式：悬停放大 + 蓝色光晕 -->
<style>
  .hover-blue-glow {
    display: inline-block;
    font-size: 28px;
    font-weight: bold;
    color: #006666;
    background: rgba(255, 255, 200, 0.8);
    padding: 12px 28px;
    border-radius: 48px;
    cursor: pointer;
    transition: transform 0.4s cubic-bezier(0.2, 0.9, 0.4, 1.1), text-shadow 0.3s ease;
    margin-top: 20px;
  }
  .hover-blue-glow:hover {
    transform: scale(1.2);
    text-shadow: 0 0 8px #00aaff, 0 0 15px #0088ff, 0 0 25px #0066cc;
  }
</style>

<!-- 3. 应用样式的文字 -->
<div class="hover-blue-glow">人皆知有用之用</div>
