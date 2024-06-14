import * as THREE from 'three';

class Match extends HTMLElement {
  constructor() {
    super();
    this._shadowRoot = this.attachShadow({ 'mode': 'open' });
    this.objects = [];
    this.speed = 2.5;
    this.height = 3;
    this.offset = 0.5;
  }

  connectedCallback() {
    this.init();
  }

  init() {
    this._shadowRoot.innerHTML = '';
    
    const container = document.createElement('div');
    this._shadowRoot.appendChild(container);
    this.scene = new THREE.Scene();
    this.clock = new THREE.Clock();
    this.camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 0.1, 100);
    this.camera.position.set(7, 3, 7);

    // Lights
    const ambientLight = new THREE.AmbientLight(0xcccccc);
    this.scene.add(ambientLight);

    const directionalLight = new THREE.DirectionalLight(0xffffff, 2.5);
    directionalLight.position.set(0, 5, 5);
    this.scene.add(directionalLight);

    const d = 5;
    directionalLight.castShadow = true;
    directionalLight.shadow.camera.left = - d;
    directionalLight.shadow.camera.right = d;
    directionalLight.shadow.camera.top = d;
    directionalLight.shadow.camera.bottom = - d;

    directionalLight.shadow.camera.near = 1;
    directionalLight.shadow.camera.far = 20;

    directionalLight.shadow.mapSize.x = 1024;
    directionalLight.shadow.mapSize.y = 1024;

    // Floor
    const floorGeometry = new THREE.PlaneGeometry(10, 10);
    const floorMaterial = new THREE.MeshLambertMaterial({ color: 0x4676b6 });

    const floor = new THREE.Mesh(floorGeometry, floorMaterial);
    floor.rotation.x = Math.PI * - 0.5;
    floor.receiveShadow = true;
    this.scene.add(floor);

    // Objects
    const count = 5;
    const radius = 3;
    const ballGeometry = new THREE.SphereGeometry(0.3, 32, 16);
    ballGeometry.translate(0, 0.3, 0);
    const ballMaterial = new THREE.MeshLambertMaterial({ color: 0xcccccc });

    for (let i = 0; i < count; i ++) {
      const s = i / count * Math.PI * 2;
      const ball = new THREE.Mesh(ballGeometry, ballMaterial);
      ball.castShadow = true;
      ball.userData.down = false;

      ball.position.x = radius * Math.cos(s);
      ball.position.z = radius * Math.sin(s);

      this.scene.add(ball);
      this.objects.push(ball);
    }

    // Renderer
    this.renderer = new THREE.WebGLRenderer({ antialias: true });
    this.renderer.setPixelRatio(window.devicePixelRatio);
    this.renderer.setSize(window.innerWidth, window.innerHeight);
    this.renderer.shadowMap.enabled = true;
    container.appendChild(this.renderer.domElement);

    this.animate();

    window.addEventListener('resize', this.onWindowResize);
  }
  
  onWindowResize() {
    this.camera.aspect = window.innerWidth / window.innerHeight;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(window.innerWidth, window.innerHeight);
  }

  render() {
    const time = this.clock.getElapsedTime();

    for (let i = 0; i < this.objects.length; i++) {
      const ball = this.objects[i];
      const previousHeight = ball.position.y;
      ball.position.y = Math.abs(Math.sin(i * this.offset + (time * this.speed)) * this.height);

      if (ball.position.y < previousHeight) {
        ball.userData.down = true;
      } else {
        if (ball.userData.down === true) {
          // ball changed direction from down to up
          ball.userData.down = false;
        }
      }
    }

    this.renderer.render(this.scene, this.camera);
  }

  animate() {
    requestAnimationFrame(this.animate.bind(this));
    this.render();
  }
}

window.customElements.define('match-mfe', Match);
